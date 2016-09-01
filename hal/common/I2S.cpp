#include "I2S.h"
#include "critical.h"

#if DEVICE_I2S

namespace mbed {

/* betzw - WAS
#if TRANSACTION_QUEUE_SIZE_I2S
CircularBuffer<Transaction<I2S>, TRANSACTION_QUEUE_SIZE_I2S> I2S::_transaction_buffer;
#endif
$*/

I2S* I2S::_owner = NULL;

void I2S::lock() {
    // betzw - DILEMMA: the right synchronization mechanism needs still to be found (see beyond)!
	//                  I assume here beyond in several calls to it that this lock will be (at least) a class level lock.
}

void I2S::unlock() {
    // betzw - DILEMMA: the right synchronization mechanism needs still to be found (see beyond)!
	//                  I assume here beyond in several calls to it that this lock will be (at least) a class level lock.
}

I2S::I2S(PinName dpin, PinName clk, PinName wsel, PinName fdpin, PinName mck) :
    _i2s(),
    _irq_tx(this), _irq_rx(this),
    _priority(MEDIUM),
    _dbits(16),
    _fbits(16),
    _polarity(0),
    _protocol(PHILIPS),
    _mode(MASTER_TX),
    _circular(false),
    _hz(44100) {
    // No lock needed in the constructor

    /*** betzw - DILEMMA:
     *   To whom is it up to guarantee that there will be no two SPI objects with the same
     *   pins (maybe created by two threads concurrently)?
     *   If this is considered to be possible, for sure we need some locking here, but also
     *   we need to figure out how to handle things like SPI clock enabling and
     *   above all disabling correctly!
     ***/

    // betzw - DILEMMA: lock();
    i2s_init(&_i2s, dpin, clk, wsel, fdpin, mck, _mode); // betzw: I do not think that 'i2s_init()' is (required to be) thread safe!
    aquire();
    // betzw - DILEMMA: unlock();
}

int I2S::format(int dbits, int fbits, int polarity) {
    lock();
    if (i2s_active(&_i2s)) {
    	unlock();
        return -1;
    }
    _dbits = dbits;
    _fbits = fbits;
    _polarity = polarity;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
    return 0;
}

int I2S::audio_frequency(unsigned int hz) {
    lock();
    if (i2s_active(&_i2s)) {
    	unlock();
        return -1;
    }
    _hz = hz;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
    return 0;
}

int I2S::set_protocol(i2s_bitorder_t protocol) {
    lock();
    if (i2s_active(&_i2s)) {
    	unlock();
        return -1;
    }
    _protocol = protocol;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
    return 0;
}

int I2S::set_mode(i2s_mode_t mode, bool circular) {
    lock();
    if (i2s_active(&_i2s)) {
    	unlock();
        return -1;
    }
    _mode = mode;
    _circular = circular;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
    return 0;
}

void I2S::abort_transfer()
{
    lock();
    i2s_abort_asynch(&_i2s);
#if TRANSACTION_QUEUE_SIZE_I2S
    dequeue_transaction();
#endif
    unlock();
}


void I2S::clear_transfer_buffer()
{
#if TRANSACTION_QUEUE_SIZE_I2S
    lock();
    _transaction_buffer.reset();
    unlock();
#endif
}

void I2S::abort_all_transfers()
{
    lock();
    clear_transfer_buffer();
    abort_transfer();
    unlock();
}

int I2S::get_transfer_status()
{
    lock();
    if (i2s_active(&_i2s)) {
    	unlock();
        return -1;
    }
    unlock();
    return  0;
}

unsigned int I2S::get_module()
{
    return i2s_get_module(&_i2s);
}

int I2S::set_dma_priority(i2s_dma_prio_t prio)
{
    lock();
    if (i2s_active(&_i2s)) {
    	unlock();
        return -1;
    }
    _priority = prio;
    unlock();
    return  0;
}

int I2S::queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, const event_callback_t& callback, int event)
{
#if TRANSACTION_QUEUE_SIZE_I2S
    transaction_t t;

    t.tx_buffer = const_cast<void *>(tx_buffer);
    t.tx_length = tx_length;
    t.rx_buffer = rx_buffer;
    t.rx_length = rx_length;
    t.event = event;
    t.callback = callback;
    t.width = 16;
    Transaction<I2S> transaction(this, t);
    core_util_critical_section_enter();
    if (_transaction_buffer.full()) {
        core_util_critical_section_enter();
        return -1; // the buffer is full
    } else {
        _transaction_buffer.push(transaction);
        core_util_critical_section_exit();
        dequeue_transaction();
        return 0;
    }
#else
    return -1;
#endif
}


/***
 *
 * betzw - NOTE: ALL the following functions MIGHT be called in interrupt context, so pay attention (e.g. no mutex locking)!!!
 *
 * This is kind of a DILEMMA as at least some of these functions should be synchronized with all the above ones thru the
 * same synchronization mechanism. But being in interrupt context a PlatformMutex is no option anymore, so everywhere
 * we would need some IRQ enable/disable mechanism which for sure is impacting performance, irq latencies, and real-time
 * behavior too heavily, and limits what can be done within such critical sections very very much!
 *
 * The only solution which comes to my mind is splitting somehow the IRQ handlers in top and bottom halves, where:
 * The so-called top-half is the routine that actually responds to the interrupt (i.e. the actual interrupt handler).
 * The bottom-half is a routine that is scheduled by the top-half to be executed later, at a "safer time", and which
 * performs most of the often substantial amount of work that must be done in response to a device interrupt.
 * The big difference between the top-half handler and the bottom-half is that all interrupts are enabled during execution
 * of the bottom-half - that’s why it runs at a "safer time".
 * In a typical scenario, the top-half saves device data to a device-specific buffer, schedules its bottom-half,
 * and exits: this operation is very fast. Note: interrupt handlers need to finish up quickly and not keep interrupts
 * blocked for long!
 * The bottom-half then performs whatever other work is required, such as starting up another I/O operation, perform
 * housekeeping, and so on.
 * This setup permits the top-halves to service a new interrupt while bottom-halves might still be working.
 * Almost every serious interrupt handler is split this way!
 * For instance, when a network interface reports the arrival of a new packet, the handler just retrieves the data and
 * pushes it up to the protocol layer; actual processing of the packet is performed in a bottom-half.
 *
 * So we need to find a general way on how to schedule bottom-halves to be executed at a "safer time" in mbed-os!
 *
 ***/

// ignore the fact there are multiple physical i2s's, and always update if it wasn't us last
void I2S::aquire() { // betzw - DILEMMA: MUST be called with lock held?!?
    // betzw - TODO: evtl. assert here that lock is held!

    if (_owner != this) {
    	i2s_format(&_i2s, _dbits, _fbits, _polarity);
        i2s_audio_frequency(&_i2s, _hz);
        i2s_set_protocol(&_i2s, _protocol);
        i2s_set_mode(&_i2s, _mode);
        _owner = this;
    }
}

void I2S::start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, 
			 const event_callback_t& callback, int event)
{
    // betzw - DILEMMA: lock();
    aquire();
    _callback = callback;
    _irq_tx.callback(&I2S::irq_handler_asynch_tx);
    _irq_rx.callback(&I2S::irq_handler_asynch_rx);
    i2s_transfer(&_i2s,
		 const_cast<void *>(tx_buffer), tx_length, rx_buffer, rx_length,
		 _circular, _priority,
		 _irq_tx.entry(), _irq_rx.entry(),
		 event);
    // betzw - DILEMMA: unlock();
}

#if TRANSACTION_QUEUE_SIZE_I2S

void I2S::start_transaction(transaction_t *data)
{
    start_transfer(data->tx_buffer, data->tx_length, data->rx_buffer, data->rx_length, data->callback, data->event);
}

void I2S::dequeue_transaction()
{
    // betzw - DILEMMA: lock();
    if (!i2s_active(&_i2s)) {
    	Transaction<I2S> t;
    	if (_transaction_buffer.pop(t)) {
    		I2S* obj = t.get_object();
    		transaction_t* data = t.get_transaction();
    		MBED_ASSERT(obj == this); // betzw: what if 'obj' is NOT equal to 'this'?
    		obj->start_transaction(data);
    	}
    }
    // betzw - DILEMMA: unlock();
}

#endif

/***
 * betzw - TODO: the beyond irq handlers must be split in top and bottom halves (see above).
 *               Above all the '_callback' and 'dequeue_transaction()' shall be executed in
 *               the bottom half!
 ***/
void I2S::irq_handler_asynch_rx(void)
{
    int event = i2s_irq_handler_asynch(&_i2s, I2S_RX_EVENT);
    if (_callback && (event & I2S_EVENT_ALL)) {
        _callback.call(event & I2S_EVENT_ALL); // betzw - TODO: schedule as bottom half
    }
#if TRANSACTION_QUEUE_SIZE_I2S
    if (event & I2S_EVENT_INTERNAL_TRANSFER_COMPLETE) {
    	dequeue_transaction(); // betzw - TODO: schedule as bottom half
    }
#endif
}

void I2S::irq_handler_asynch_tx(void)
{
    int event = i2s_irq_handler_asynch(&_i2s, I2S_TX_EVENT);
    if (_callback && (event & I2S_EVENT_ALL)) {
        _callback.call(event & I2S_EVENT_ALL); // betzw - TODO: schedule as bottom half
    }
#if TRANSACTION_QUEUE_SIZE_I2S
    if (event & I2S_EVENT_INTERNAL_TRANSFER_COMPLETE) {
    	dequeue_transaction(); // betzw - TODO: schedule as bottom half
    }
#endif
}

} // namespace mbed

#endif
