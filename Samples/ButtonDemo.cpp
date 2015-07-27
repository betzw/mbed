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
#include "ButtonService.h"

#include "Samples.h"

#ifdef BUTTON_DEMO_ENABLED

BLE         ble;
DigitalOut  led1(LED1);
InterruptIn button(USER_BUTTON);

const static char     DEVICE_NAME[] = "Button";
static const uint16_t uuid16_list[] = {ButtonService::BUTTON_SERVICE_UUID};

ButtonService *buttonServicePtr;

void buttonPressedCallback(void)
{
    buttonServicePtr->updateButtonState(true);
}

void buttonReleasedCallback(void)
{
    buttonServicePtr->updateButtonState(false);
}

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.gap().startAdvertising();
}

void periodicCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 to indicate system aliveness. */
}

void buttonDemo(void)
{
    led1 = 1;
    Ticker ticker;
    ticker.attach(periodicCallback, 1);
    button.fall(buttonPressedCallback);
    button.rise(buttonReleasedCallback);

    ble.init();
    ble.gap().onDisconnection(disconnectionCallback);

    ButtonService buttonService(ble, false /* initial value for button pressed */);
    buttonServicePtr = &buttonService;

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

#endif /* SENSOR_DEMO_ENABLED */