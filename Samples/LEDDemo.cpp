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
#include "LEDService.h"
#include "Utils.h"

#include "x_nucleo_idb0xa1_targets.h"
#include "Samples.h"

#ifdef LED_DEMO_ENABLED

#if defined(IDB0XA1_D13_PATCH)
#error "LED demo not available with patch D13 applied on IDB0XA1 expansion boards!"
#endif // defined(IDB0XA1_D13_PATCH)

BLE        ble;
DigitalOut actuatedLED(LED1);

const static char     DEVICE_NAME[] = "LED";
static const uint16_t uuid16_list[] = {LEDService::LED_SERVICE_UUID};

LEDService *ledServicePtr;

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    ble.gap().startAdvertising();
}

void periodicCallback(void)
{
    //alivenessLED = !alivenessLED; /* Do blinky on LED1 to indicate system aliveness. */
}

/**
 * This callback allows the LEDService to receive updates to the ledState Characteristic.
 *
 * @param[in] params
 *     Information about the characterisitc being updated.
 */
void onDataWrittenCallback(const GattWriteCallbackParams *params) {
    DEBUG("params->handle=%d ledServicePtr->getValueHandle()=%d\n\r", params->handle, ledServicePtr->getValueHandle());
    if ((params->handle == ledServicePtr->getValueHandle()) && (params->len == 1)) {
        actuatedLED = *(params->data);
    }
}

void ledDemo(void)
{
    actuatedLED  = 0;

    Ticker ticker;
    ticker.attach(periodicCallback, 1);

    ble.init();
    ble.gap().onDisconnection(disconnectionCallback);
    ble.gattServer().onDataWritten(onDataWrittenCallback);

    bool initialValueForLEDCharacteristic = false;
    LEDService ledService(ble, initialValueForLEDCharacteristic);
    ledServicePtr = &ledService;

    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms. */
    ble.gap().startAdvertising();

    while (true) {
        ble.waitForEvent();
    }
}

#endif /* LED_DEMO_ENABLED */
