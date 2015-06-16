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
#include "HeartRateService.h"
#include "DeviceInformationService.h"
 
#include "Samples.h"

#ifdef HRM_SAMPLE_ENABLED

/* Enable the following if you need to throttle the connection interval. This has
 * the effect of reducing energy consumption after a connection is made;
 * particularly for applications where the central may want a fast connection
 * interval.*/
#define UPDATE_PARAMS_FOR_LONGER_CONNECTION_INTERVAL 0
 
BLEDevice  ble;
DigitalOut led1(LED1);
 
const static char     DEVICE_NAME[]        = "HRM";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEART_RATE_SERVICE,
                                              GattService::UUID_DEVICE_INFORMATION_SERVICE};
static volatile bool  triggerSensorPolling = false;
 
void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising(); // restart advertising
}
 
void periodicCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */
 
    /* Note that the periodicCallback() executes in interrupt context, so it is safer to do
     * heavy-weight sensor polling from the main thread. */
    triggerSensorPolling = true;
}
 
void hrmSample(void)
{
    led1 = 1;
    Ticker ticker;
    ticker.attach(periodicCallback, 1); // blink LED every second

    ble.init();
    ble.onDisconnection(disconnectionCallback);
 
    /* Setup primary service. */
    uint8_t hrmCounter = 60; // init HRM to 60bps
    HeartRateService hrService(ble, hrmCounter, HeartRateService::LOCATION_CHEST);
 
    /* Setup auxiliary service. */
    DeviceInformationService deviceInfo(ble, "STM", "Model1", "SN1", "hw-rev1", "fw-rev1", "soft-rev1");
 
    /* Setup advertising. */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_HEART_RATE_SENSOR);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(1000);
    ble.startAdvertising();
 
    // infinite loop
    while (1) {
        // check for trigger from periodicCallback()
        if (triggerSensorPolling && ble.getGapState().connected) {
            triggerSensorPolling = false;
 
            // Do blocking calls or whatever is necessary for sensor polling.
            // In our case, we simply update the HRM measurement. 
            hrmCounter = (rand()&0x1F)+60;
            
            // update bpm
            hrService.updateHeartRate(hrmCounter);
        } else {
            ble.waitForEvent(); // low power wait for event
        }
    }
}
 
#endif /* HRM_SAMPLE_ENABLED */