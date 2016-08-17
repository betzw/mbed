#ifndef MBED_I2S_H
#define MBED_I2S_H

#include "platform.h"

#if DEVICE_I2S

#include "i2s_api.h"
#include "CThunk.h"
#include "dma_api.h"
#include "CircularBuffer.h"
#include "FunctionPointer.h"
#include "Transaction.h"

namespace mbed {

/** A I2S Master/Slave, used for communicating with I2S slave/master devices
 *
 * The default format is set to master transmission mode, 16 data bits & 16 bits per frame,
 * clock polarity 0, protocol PHILIPS, and a clock frequency of 44.1kHz
 *
 * TODO: "direct" PDM support
 *
 * NOTE: This information will be deprecated soon.
 * Most I2S devices will also require Reset signals. These
 * can be controlled using <DigitalOut> pins
 */
class I2S {

public:
    /** I2S transfer callback
     *  @param Buffer the tx buffer
     *  @param Buffer the rx buffer
     *  @param int the event that triggered the callbacki2s_init
     */
    typedef Callback<void(int)> event_callback_t;
    typedef struct
    {
        bool _circular;
        i2s_dma_prio_t _priority;
    } i2s_transaction_data_t;

public:
    /** Create a I2S master connected to the specified pins
     *
     *  @param dpin  I2S data input/output pin
     *  @param clk   I2S clock output pin
     *  @param wsel  I2S word select output pin (might be NC for PDM sources)
     *  @param fdpin I2S data input pin (for full-duplex operation, default = NC)
     *  @param mck   I2S master clock output (additional pin when needed for some external audio devices, default = NC)
     */
    I2S(PinName dpin, PinName clk, PinName wsel, PinName fdpin = NC, PinName mck = NC);

    /** Configure the data transmission format
     *
     *  @param dbits Number of data bits per I2S frame (16, 24, or 32)
     *  @param fbits Number of bits per I2S frame (16 or 32)
     *  @param polarity Clock polarity (either 0/low or 1/high, default = 0)
     */
    void format(int dbits, int fbits, int polarity = 0);

    /** Set the i2s audio frequency
     *
     *  @param hz audio frequency in hz
     */
    void audio_frequency(unsigned int hz);

    /** Set the i2s bus protocol
     *
     *  @param protocol I2S protocol to be used
     *  @return Zero if the usage was set, -1 if a transaction is on-going
     */
    void set_protocol(i2s_bitorder_t protocol);

    /** Set the i2s mode
     *
     *  @param mode I2S mode to be used
     *  @return Zero if the usage was set, -1 if a transaction is on-going
     */
    void set_mode(i2s_mode_t mode);

    /** Start non-blocking I2S transfer using 8bit buffers.
     *
     * @param tx_buffer The TX buffer with data to be transfered. If NULL is passed,
     *                  the default I2S value is sent.
     * @param tx_length The length of TX buffer in bytes
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param callback  The event callback function.
     * @param event     The logical OR of events to modify. Look at i2s hal header file for I2S events.
     * @param td        The pointer to the I2S transaction's data.
     * @return Zero if the transfer has started, or -1 if I2S peripheral is busy.
     */
    template<typename Type>
    int transfer(const Type *tx_buffer, int tx_length, Type *rx_buffer, int rx_length, const event_callback_t& callback, int event/* = I2S_EVENT_COMPLETE*/, i2s_transaction_data_t *td) {
        if (i2s_active(&_i2s)) {
            return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, sizeof(Type) * 8, callback, event, td);
        }
        start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, sizeof(Type) * 8, callback, event, td);
        return 0;
    }

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
     *
     * @param tx_buffer The TX buffer with data to be transfered. If NULL is passed,
     *                  the default I2S value is sent.
     * @param tx_length The length of TX buffer in bytes
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param bit_width The width in bits of the data;
     * @param callback  The event callback function.
     * @param event     The logical OR of events to modify. Look at i2s hal header file for I2S events.
     * @param td        The pointer to the I2S transaction's data.
     * @return Zero if a transfer was added to the queue, or -1 if the queue is full.
     */
    int queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td);

    /** Configures a callback, i2s peripheral and initiate a new transfer
     *
     * @param data Transaction data
     *                  the default I2S value is sent.
     * @param tx_length The length of TX buffer in bytes
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param bit_width The width in bits of the data;
     * @param callback  The event callback function.
     * @param event     The logical OR of events to modify. Look at i2s hal header file for I2S events.
     * @param td        The pointer to the I2S transaction's data.
     */
    void start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td);

#if TRANSACTION_QUEUE_SIZE_I2S
    /** Start a new transaction
     *
     *  @param data Transaction data
     */
    void start_transaction(transaction_t *t)

    /** Dequeue a transaction
     *
     */
    void dequeue_transaction();
#endif // TRANSACTION_QUEUE_SIZE_I2S

    /** Initiate a transfer
     *
     * @param tx_buffer The TX buffer with data to be transfered. If NULL is passed,
     *                  the default I2S value is sent.
     * @param tx_length The length of TX buffer in bytes
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param bit_width The width in bits of the data;
     * @param callback  The event callback function.
     * @param event     The logical OR of events to modify. Look at i2s hal header file for I2S events.
     * @param td        The pointer to the I2S transaction's data.
     * @return The result of validating the transfer parameters.
     */
    int transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td);

public:
    virtual ~I2S() {
    	/* betzw - TODO: cleanup has still to be revised completely! */
    	abort_all_transfers();
    	i2s_free(&_i2s);
    }

protected:
    i2s_t _i2s;

#if TRANSACTION_QUEUE_SIZE_I2S
    static CircularBuffer<transaction_t, TRANSACTION_QUEUE_SIZE_I2S> _transaction_buffer;
#endif
    CThunk<I2S> _irq_tx;
    CThunk<I2S> _irq_rx;
    transaction_t _current_transaction;

    void aquire(void);

    static I2S *_owner;
    int _dbits;
    int _fbits;
    int _polarity;
    i2s_bitorder_t _protocol;
    i2s_mode_t _mode;
    bool _busy;
    unsigned int _hz;
};

} // namespace mbed

#endif

#endif
