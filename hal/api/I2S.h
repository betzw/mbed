#ifndef MBED_I2S_H
#define MBED_I2S_H

#include "platform.h"

#if DEVICE_I2S

#ifndef MBED_CONF_RTOS_PRESENT
#error "I2S only supports asynchronous transfers which require a RTOS!"
#endif

#include "PlatformMutex.h"
#include "i2s_api.h"
#include "SingletonPtr.h"

#include "CThunk.h"
#include "dma_api.h"
#include "CircularBuffer.h"
#include "FunctionPointer.h"
#include "Transaction.h"

#include "Thread.h"
#include "EventQueue.h"

namespace mbed {

/** A I2S Master/Slave, used for communicating with I2S slave/master devices
 *
 * The default format is set to master transmission mode, one-shot (i.e. not circular) 
 * 16 data bits & 16 bits per frame, clock polarity 0, 
 * protocol PHILIPS, and a clock frequency of 44.1kHz
 *
 * TODO: "direct" PDM support
 *
 * Most I2S devices will also require Reset signals. These
 * can be controlled using <DigitalOut> pins
 *
 * @Note Synchronization level: Thread safe
 *
 */
class I2S {

public:
    /** Create a I2S master connected to the specified pins
     *
     *  @param dpin  I2S data input/output pin
     *  @param clk   I2S clock output pin
     *  @param wsel  I2S word select output pin (might be NC for PDM sources)
     *  @param fdpin I2S data input pin (for full-duplex operation, default = NC)
     *  @param mck   I2S master clock output (additional pin when needed for some external audio devices, default = NC)
     *
     *  @Note It is up to the application programmer to not generate at the same time two I2S instances with the
     *        same pin (NC excluded) for one the parameters, otherwise the correct operation of this class cannot be
     *        guaranteed (e.g. things like SPI clock enabling and above all disabling might not work correctly)!
     */
    I2S(PinName dpin, PinName clk, PinName wsel, PinName fdpin = NC, PinName mck = NC);

    /** Configure the data transmission format
     *
     *  @param dbits Number of data bits per I2S frame (16, 24, or 32)
     *  @param fbits Number of bits per I2S frame (16 or 32)
     *  @param polarity Clock polarity (either 0/low or 1/high, default = 0)
     *  @return Zero if the usage was set, -1 if a transaction is on-going
     */
    int format(int dbits, int fbits, int polarity = 0);

    /** Set the i2s audio frequency
     *
     *  @param hz audio frequency in hz
     *  @return Zero if the usage was set, -1 if a transaction is on-going
     */
    int audio_frequency(unsigned int hz);

    /** Set the i2s bus protocol
     *
     *  @param protocol I2S protocol to be used
     *  @return Zero if the usage was set, -1 if a transaction is on-going
     */
    int set_protocol(i2s_bitorder_t protocol);

    /** Set the i2s mode
     *
     *  @param mode     I2S mode to be used
     *  @param circular I2S should read/write buffers continously (in circular mode)
     *  @return Zero if the usage was set, -1 if a transaction is on-going
     */
    int set_mode(i2s_mode_t mode, bool circular);

    /** Start non-blocking I2S transfer as configured with above methods
     *
     * @param tx_buffer The TX buffer with data to be transfered. If NULL is passed,
     *                  no transmission will be set up 
     * @param tx_length The length of TX buffer in bytes
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data will be ignored
     * @param rx_length The length of RX buffer in bytes
     * @param callback  The event callback function
     *                  NOTE: this one will be executed in interrupt context!!!
     * @param event     The logical OR of events to notify. Look at i2s hal header file for I2S events.
     * @return Zero if the transfer has started (or been queued), or 
     *         -1   if I2S peripheral is busy (or out of resources)
     */
    template<typename Type>
    int transfer(const Type *tx_buffer, int tx_length, Type *rx_buffer, int rx_length, const event_callback_t& callback, int event) {
    	int ret = 0;

    	lock();
#if !TRANSACTION_QUEUE_SIZE_I2S
    	if(callback && (event & I2S_EVENT_ALL)) {
    	    /* Init bottom half daemon */
    	    I2sBhHandler::init();
    	}
#endif

    	if (i2s_active(&_i2s)
#if TRANSACTION_QUEUE_SIZE_I2S
    			|| !_transaction_buffer.empty()
#endif
    	) {
    		ret = queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, callback, event);
    	} else {
    		start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, callback, event);
    	}
    	unlock();
    	return ret;
    }
    
    /** Acquire exclusive access to this I2S bus
     */
    virtual void lock(void);

    /** Release exclusive access to this I2S bus
     */
    virtual void unlock(void);

    /** Abort the on-going I2S transfer, and continue with transfer's in the queue if any.
     */
    void abort_transfer();

    /** Clear the transaction buffer
     */
    void clear_transfer_buffer();

    /** Clear the transaction buffer and abort on-going transfer.
     */
    void abort_all_transfers();

    /** Get transfer status
     *
     *  @return -1 if a transaction is on-going, zero otherwise
     */
    int get_transfer_status();

    /** Get internal module id
     *
     *  @return internal module id
     */
    unsigned int get_module();

    /** Configure DMA priority for transfers
     *
     *  @param prio The DMA priority to be used
     *  @return Zero if the usage was set, -1 if a transaction is on-going
    */
    int set_dma_priority(i2s_dma_prio_t prio);

protected:
    /** I2S TX DMA IRQ handler
     *
     */
    void irq_handler_asynch_tx(void);

    /** I2S RX DMA IRQ handler
     *
     */
    void irq_handler_asynch_rx(void);

    /** Add a transfer to the queue
     * @param data Transaction data
     * @return Zero if a transfer was added to the queue, or -1 if the queue is full
     */
    int queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, 
		       const event_callback_t& callback, int event);
    
    /** Configures a callback, i2s peripheral and initiate a new transfer
     *
     * @param data Transaction data
     */
    void start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, 
			const event_callback_t& callback, int event);

    class I2sBhHandler {
    	friend class I2S;

    	static void init() {
    		static bool inited = false;

    		if(!inited) {
    			Callback<void()> i2s_bh_task(&_i2s_bh_queue, &events::EventQueue::dispatch);
    			_i2s_bh_daemon.start(i2s_bh_task);
    			inited = true;
    		}
    	}

    	static void i2s_defer_function(const event_callback_t& bottom_half, int event) {
    		_i2s_bh_queue.call(bottom_half, event);
    	}

    	static void i2s_defer_function(const Callback<void()>& bottom_half) {
    		_i2s_bh_queue.call(bottom_half);
    	}

    	static rtos::Thread _i2s_bh_daemon;
    	static events::EventQueue _i2s_bh_queue;
    };

#if TRANSACTION_QUEUE_SIZE_I2S
    /** Start a new transaction
     *
     *  @param data Transaction data
     */
    void start_transaction(transaction_t *data);

    /** Dequeue a transaction
     *
     */
    void dequeue_transaction();

    /* betzw - WAS : static */ CircularBuffer<Transaction<I2S>, TRANSACTION_QUEUE_SIZE_I2S> _transaction_buffer;
#endif // TRANSACTION_QUEUE_SIZE_I2S

public:
    virtual ~I2S() {
    	/* betzw - TODO: cleanup has still to be revised completely! */
    	abort_all_transfers();
    	i2s_free(&_i2s);
    }

protected:
    i2s_t _i2s;

    CThunk<I2S> _irq_tx;
    CThunk<I2S> _irq_rx;
    event_callback_t _callback;
    i2s_dma_prio_t _priority; // DMA priority

    void acquire(void);

    static I2S *_owner;
    static SingletonPtr<PlatformMutex> _mutex;

    int _dbits;
    int _fbits;
    int _polarity;
    i2s_bitorder_t _protocol;
    i2s_mode_t _mode;
    bool _circular;
    unsigned int _hz;
};

} // namespace mbed

#endif

#endif
