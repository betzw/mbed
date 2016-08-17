#include "I2S.h"
#include "mbed_assert.h"
#include "critical.h"

#if DEVICE_I2S

namespace mbed {

//using namespace util;

#if TRANSACTION_QUEUE_SIZE_I2S
CircularBuffer<I2S::transaction_t, TRANSACTION_QUEUE_SIZE_I2S> I2S::_transaction_buffer;
#endif

I2S::I2S(PinName dpin, PinName clk, PinName wsel, PinName fdpin, PinName mck) :
        _i2s(),
	_irq_tx(this), _irq_rx(this),
        _dbits(16),
        _fbits(16),
	_polarity(0),
	_protocol(PHILIPS),
	_mode(MASTER_TX),
	_busy(false),
        _hz(44100) {
    i2s_init(&_i2s, dpin, clk, wsel, fdpin, mck, _mode);
    i2s_format(&_i2s, _dbits, _fbits, _polarity);
    i2s_audio_frequency(&_i2s, _hz);
    i2s_set_protocol(&_i2s, _protocol);
}

void I2S::format(int dbits, int fbits, int polarity) {
    _dbits = dbits;
    _fbits = fbits;
    _polarity = polarity;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
}

void I2S::audio_frequency(unsigned int hz) {
    _hz = hz;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
}

void I2S::set_protocol(i2s_bitorder_t protocol) {
    _protocol = protocol;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
}

void I2S::set_mode(i2s_mode_t mode) {
	_mode = mode;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
}

I2S* I2S::_owner = NULL;

// ignore the fact there are multiple physical i2ss, and always update if it wasn't us last
void I2S::aquire() {
     if (_owner != this) {
    	i2s_format(&_i2s, _dbits, _fbits, _polarity);
        i2s_audio_frequency(&_i2s, _hz);
        i2s_set_protocol(&_i2s, _protocol);
        i2s_set_mode(&_i2s, _mode);
        _owner = this;
    }
}

int I2S::transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td)
{
    if (i2s_active(&_i2s)) {
        return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event, td);
    }
    start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event, td);
    return 0;
}

void I2S::abort_transfer()
{
    i2s_abort_asynch(&_i2s);
#if TRANSACTION_QUEUE_SIZE_I2S
    dequeue_transaction();
#endif
}


void I2S::clear_transfer_buffer()
{
#if TRANSACTION_QUEUE_SIZE_I2S
    _transaction_buffer.reset();
#endif
}

void I2S::abort_all_transfers()
{
    clear_transfer_buffer();
    abort_transfer();
}

int I2S::get_transfer_status()
{
    if (i2s_active(&_i2s)) {
        return -1;
    }
    return  0;
}

unsigned int I2S::get_module()
{
	return i2s_get_module(&_i2s);
}

int I2S::queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td)
{
#if TRANSACTION_QUEUE_SIZE_I2S
    transaction_t t;

    t.tx_buffer = const_cast<void *>(tx_buffer);
    t.tx_length = tx_length;
    t.rx_buffer = rx_buffer;
    t.rx_length = rx_length;
    t.event = event;
    t.callback = callback;
    t.width = bit_width;
    t.data = (i2s_transaction_data_t *) td;
    Transaction<SPI> transaction(this, t);
    if (_transaction_buffer.full()) {
        return -1; // the buffer is full
    } else {
        core_util_critical_section_enter();
        _transaction_buffer.push(transaction);
        if (!i2s_active(&_i2s)) {
            dequeue_transaction();
        }
        core_util_critical_section_exit();
        return 0;
    }
#else
    return -1;
#endif
}

//void I2S::start_transfer(const transaction_t &t)
void I2S::start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td)
{
    aquire();
    _current_transaction.tx_buffer = (void *) tx_buffer;
    _current_transaction.tx_length = tx_length;
    _current_transaction.rx_buffer = (void *) rx_buffer;
    _current_transaction.rx_length = rx_length;
    _current_transaction.event = event;
    _current_transaction.callback = callback;
    _current_transaction.width = bit_width;
    _current_transaction.data = (void *) td;
    _irq_tx.callback(&I2S::irq_handler_asynch_tx);
    _irq_rx.callback(&I2S::irq_handler_asynch_rx);
    i2s_transfer(&_i2s,
    		_current_transaction.tx_buffer, _current_transaction.tx_length,
			_current_transaction.rx_buffer, _current_transaction.rx_length,
			(*((i2s_transaction_data_t *) _current_transaction.data))._circular, (*((i2s_transaction_data_t *) _current_transaction.data))._priority,
            _irq_tx.entry(), _irq_rx.entry(), _current_transaction.event);
}

#if TRANSACTION_QUEUE_SIZE_I2S

void I2S::start_transaction(transaction_t *t)
{
    start_transfer(t->tx_buffer, t->tx_length, t->rx_buffer, t->rx_length, t->width, t->callback, t->event, (i2s_transaction_data_t *) t->data));
}

void I2S::dequeue_transaction()
{
    transaction_t t;
    bool dequeued;
    {
        CriticalSectionLock lock;
        dequeued = _transaction_buffer.pop(t);
        _busy = dequeued;
    }

    if (dequeued) {
        I2S* obj = t.get_object();
        transaction_t* t = t.get_transaction();
        obj->start_transaction(t);
    }
}

#endif // TRANSACTION_QUEUE_SIZE_I2S

void I2S::irq_handler_asynch_rx(void)
{
    int event = i2s_irq_handler_asynch(&_i2s, I2S_RX_EVENT);
    if (_current_transaction.callback && (event & I2S_EVENT_ALL)) {
        _current_transaction.callback.call(event & I2S_EVENT_ALL);
    }
#if TRANSACTION_QUEUE_SIZE_I2S
    if (event & (I2S_EVENT_ALL | I2S_EVENT_INTERNAL_TRANSFER_COMPLETE)) {
        dequeue_transaction();
    }
#endif
}

void I2S::irq_handler_asynch_tx(void)
{
    int event = i2s_irq_handler_asynch(&_i2s, I2S_TX_EVENT);
    if (_current_transaction.callback && (event & I2S_EVENT_ALL)) {
        _current_transaction.callback.call(event & I2S_EVENT_ALL);
    }
#if TRANSACTION_QUEUE_SIZE_I2S
    if (event & (I2S_EVENT_ALL | I2S_EVENT_INTERNAL_TRANSFER_COMPLETE)) {
        dequeue_transaction();
    }
#endif
}

} // namespace mbed

#endif
