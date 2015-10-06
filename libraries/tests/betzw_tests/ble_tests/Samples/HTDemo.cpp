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
#include "BLE.h"
#include "HealthThermometerService.h"
 
#include "x_nucleo_idb0xa1_targets.h"
#include "Samples.h"

#ifdef HT_DEMO_ENABLED

BLE        ble;

#if !defined(IDB0XA1_D13_PATCH)
DigitalOut led1(LED1);
#endif
 
const static char     DEVICE_NAME[]        = "Therm";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEALTH_THERMOMETER_SERVICE};
static volatile bool  triggerSensorPolling = false;
 
/* Restart Advertising on disconnection*/
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    ble.gap().startAdvertising();
}
 
void periodicCallback(void)
{
#if !defined(IDB0XA1_D13_PATCH)
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */
#endif
 
    /* Note that the periodicCallback() executes in interrupt context, so it is safer to do
     * heavy-weight sensor polling from the main thread. */
    triggerSensorPolling = true;
}
 
void htDemo(void)
{
#if !defined(IDB0XA1_D13_PATCH)
    led1 = 1;
#endif
    Ticker ticker;
    ticker.attach(periodicCallback, 1);
 
    ble.init();
    ble.gap().onDisconnection(disconnectionCallback);
 
    /* Setup primary service. */
    float currentTemperature = 39.6;
    HealthThermometerService thermometerService(ble, currentTemperature, HealthThermometerService::LOCATION_EAR);
 
    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::THERMOMETER_EAR);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    ble.gap().startAdvertising();
 
    while (true) {
        if (triggerSensorPolling && ble.getGapState().connected) {
            triggerSensorPolling = false;
 
            /* Do blocking calls or whatever is necessary for sensor polling. */
            // error = sensor.readData();
            // if (!error) {
            //     thermometerService.updateTemperature(c);
            // }
 
            /* In our case, we simply update the dummy temperature measurement. */
            currentTemperature += 0.1;
            thermometerService.updateTemperature(currentTemperature);
        } else {
            ble.waitForEvent();
        }
    }
}

#endif /* HRM_DEMO_ENABLED */
