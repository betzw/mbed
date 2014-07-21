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
#include "BlueNRGDevice.h"

#include "btle.h"

/**
 * The singleton which represents the nRF51822 transport for the BLEDevice.
 */
static BlueNRGDevice deviceInstance;

/**
 * BLE-API requires an implementation of the following function in order to
 * obtain its transport handle.
 */
BLEDeviceInstanceBase *
createBLEDeviceInstance(void)
{
    return (&deviceInstance);
}

/**************************************************************************/
/*!
    @brief  Constructor
*/
/**************************************************************************/
BlueNRGDevice::BlueNRGDevice(void)
{
}

/**************************************************************************/
/*!
    @brief  Destructor
*/
/**************************************************************************/
BlueNRGDevice::~BlueNRGDevice(void)
{
}

/**************************************************************************/
/*!
    @brief  Initialises anything required to start using BLE

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGDevice::init(void)
{
    /* ToDo: Clear memory contents, reset the SD, etc. */
    btle_init();

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Resets the BLE HW, removing any existing services and
            characteristics

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGDevice::reset(void)
{
    wait(0.5);

    /* Reset BlueNRG SPI interface */
    //BlueNRG_RST();
  
    /* Wait for the radio to come back up */
    wait(1);

    return BLE_ERROR_NONE;
}

void BlueNRGDevice::waitForEvent(void)
{
    //sd_app_evt_wait();
    HCI_Process();//Send App Events??
    
}
