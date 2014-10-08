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

#include "mbed.h"
#include "BLEDevice.h"

BLEDevice ble;

static const uint8_t BEACON_UUID[] = {0xD8, 0xFE};
uint8_t urldata[] =
{
    BEACON_UUID[0], BEACON_UUID[1],
    0x00, // flags
    0x20, // power
    0x00, // http://www.
    'm',  // 'm'
    'b',  // 'b'
    'e',  // 'e'
    'd',  // 'd'
    0x08, // .".org"
};

int main(void)
{
    ble.init();

    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, BEACON_UUID, sizeof(BEACON_UUID));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, urldata, sizeof(urldata));

    ble.setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(1600); /* 1s; in multiples of 0.625ms. */
    ble.startAdvertising();

    while (true) {
        ble.waitForEvent();
    }
}
