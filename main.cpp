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
#include "URIBeacon2Service.h"
#include "DFUService.h"
#include "DeviceInformationService.h"

BLEDevice ble;

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising();
}

int main(void)
{
    ble.init();
    ble.onDisconnection(disconnectionCallback);

    URIBeacon2Service uriBeacon(ble, "http://developer.mbed.org");
    if (!uriBeacon.configuredSuccessfully()) {
        error("failed to accommodate URI");
    }
    /* optional use of the API offered by URIBeacon2Service */
    uriBeacon.setTxPowerLevel(URIBeacon2Service::TX_POWER_MODE_LOW, -4);
    uriBeacon.useTxPowerMode(URIBeacon2Service::TX_POWER_MODE_LOW);

    /* Setup auxiliary services. */
    DFUService dfu(ble); /* To allow over-the-air firmware udpates. optional. */
    DeviceInformationService deviceInfo(ble, "ARM", "URIBeacon2", "SN1", "hw-rev1", "fw-rev1", "soft-rev1"); /* optional */

    ble.startAdvertising();

    while (true) {
        ble.waitForEvent();
    }
}
