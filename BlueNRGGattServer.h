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

#ifndef __BLUENRG_GATT_SERVER_H__
#define __BLUENRG_GATT_SERVER_H__

#include "mbed.h"
#include "blecommon.h"
#include "btle.h"
#include "GattService.h"
#include "hw/GattServer.h"

#define BLE_TOTAL_CHARACTERISTICS 10

class BlueNRGGattServer : public GattServer
{
public:
    static BlueNRGGattServer &getInstance() {
        static BlueNRGGattServer m_instance;
        return m_instance;
    }

    /* Functions that must be implemented from GattServer */
    virtual ble_error_t addService(GattService &);
    virtual ble_error_t readValue(uint16_t handle, uint8_t buffer[], uint16_t *const lengthP);
    virtual ble_error_t updateValue(uint16_t, uint8_t[], uint16_t, bool localOnly = false);

    /* BlueNRG Functions */
    void eventCallback(void);
    //void hwCallback(void *pckt);

private:

    uint8_t serviceCount;
    uint8_t characteristicCount;
    tHalUint16 hrmServHandle, hrmCharHandle;
    
    GattCharacteristic *p_characteristics[BLE_TOTAL_CHARACTERISTICS];
    //uint16_t bleCharacteristicHandles[BLE_TOTAL_CHARACTERISTICS];

    BlueNRGGattServer() {
        serviceCount = 0;
        characteristicCount = 0;
    };

    BlueNRGGattServer(BlueNRGGattServer const &);
    void operator=(BlueNRGGattServer const &);
};

#endif
