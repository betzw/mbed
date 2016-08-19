/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "I2S.h"
#include "critical.h"
#include "mbed_assert.h"

#if DEVICE_I2S

namespace mbed {

#if TRANSACTION_QUEUE_SIZE_I2S
CircularBuffer<I2S::transaction_t, TRANSACTION_QUEUE_SIZE_I2S> I2S::_transaction_buffer;
#endif

I2S::I2S(PinName dpin, PinName clk, PinName wsel, PinName fdpin, PinName mck) :
    _i2s(),
    _irq_tx(this),
    _irq_rx(this),
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
    lock();
    _dbits = dbits;
    _fbits = fbits;
    _polarity = polarity;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
}

void I2S::audio_frequency(unsigned int hz) {
    lock();
    _hz = hz;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
}

void I2S::set_protocol(i2s_bitorder_t protocol) {
    lock();
    _protocol = protocol;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
}

void I2S::set_mode(i2s_mode_t mode) {
    lock();
    _mode = mode;
    I2S::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
    unlock();
}

I2S* I2S::_owner = NULL;
SingletonPtr<PlatformMutex> I2S::_mutex;

// ignore the fact there are multiple physical i2ss, and always update if it wasn't us last
void I2S::aquire() {
    lock();
    if (_owner != this) {
    	i2s_format(&_i2s, _dbits, _fbits, _polarity);
        i2s_audio_frequency(&_i2s, _hz);
        i2s_set_protocol(&_i2s, _protocol);
        i2s_set_mode(&_i2s, _mode);
        _owner = this;
    }
    unlock();
}

void I2S::lock() {
    _mutex->lock();
}

void I2S::unlock() {
    _mutex->unlock();
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
    Transaction<I2S> transaction(this, t);
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

void I2S::start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event, i2s_transaction_data_t *td)
{
    aquire();
    _callback = callback;
    _irq_tx.callback(&I2S::irq_handler_asynch_tx);
    _irq_rx.callback(&I2S::irq_handler_asynch_rx);
    i2s_transfer(&_i2s, (void *) tx_buffer, tx_length, (void *) rx_buffer, rx_length, td->_circular, td->_priority, _irq_tx.entry(), _irq_rx.entry(), event);
}

#if TRANSACTION_QUEUE_SIZE_I2S

void I2S::start_transaction(transaction_t *t)
{
    start_transfer(t->tx_buffer, t->tx_length, t->rx_buffer, t->rx_length, t->width, t->callback, t->event, (i2s_transaction_data_t *) t->data));
}

void I2S::dequeue_transaction()
{
    Transaction<I2S> t;
    if (_transaction_buffer.pop(t)) {
        I2S* obj = t.get_object();
        transaction_t* data = t.get_transaction();
        obj->start_transaction(data);
    }
}

#endif // TRANSACTION_QUEUE_SIZE_I2S

void I2S::irq_handler_asynch_rx(void)
{
    int event = i2s_irq_handler_asynch(&_i2s, I2S_RX_EVENT);
    if (_callback && (event & I2S_EVENT_ALL)) {
        _callback.call(event & I2S_EVENT_ALL);
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
    if (_callback && (event & I2S_EVENT_ALL)) {
        _callback.call(event & I2S_EVENT_ALL);
    }
#if TRANSACTION_QUEUE_SIZE_I2S
    if (event & (I2S_EVENT_ALL | I2S_EVENT_INTERNAL_TRANSFER_COMPLETE)) {
        dequeue_transaction();
    }
#endif
}

} // namespace mbed

#endif
