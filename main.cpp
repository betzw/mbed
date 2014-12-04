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
#include "URIBeaconConfigService.h"

BLEDevice ble;
URIBeaconConfigService *uriBeaconConfig;

/*If beacon gets disconnected start advertising again */
void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising();
}

/* 
*  The main loop.
*  Here we will configure the URI Beacon and start advertising 
*/
int main(void)
{
    /* Initialize BLE base layer*/
    ble.init();
    ble.onDisconnection(disconnectionCallback);

    /* Set the BLE device to be a URI Beacon, set advertising payload 
       the address after the preamble (www.) can be 17bytes at most.*/    
    uriBeaconConfig = new URIBeaconConfigService(ble, "http://www.developer.mbed.org");
    
    /*Check that the URI Beacon started successfully.*/
    if (!uriBeaconConfig->configuredSuccessfully()) {
        error("failed to accommodate URI");
    }
    
    /* optional use of the API offered by URIBeaconConfigService */
    const int8_t powerLevels[] = {-20, -4, 0, 10};
    uriBeaconConfig->setTxPowerLevels(powerLevels); // Set TX power levels, Lowest(-20), Low(-4), Medium(0), High(10)
    uriBeaconConfig->setTxPowerMode(URIBeaconConfigService::TX_POWER_MODE_LOW); // Initialize tranmition in Low power mode

    /*Start advertising the URI Beacon*/
    ble.startAdvertising();

    while (true) {
        ble.waitForEvent();
    }
}
