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

/**
  ******************************************************************************
  * @file    BlueNRGDevice.cpp 
  * @author  STMicroelectronics
  * @brief   Implementation of BLEDeviceInstanceBase
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */ 
  
/** @defgroup BlueNRGDevice
 *  @brief BlueNRG BLE_API Device Adaptation
 *  @{
 */
 
#include "mbed.h"
#include "BlueNRGDevice.h"
#include "BlueNRGGap.h"
#include "BlueNRGGattServer.h"

#include "btle.h"
#include "Utils.h"
#include "osal.h"

/**
* The singleton which represents the BlueNRG transport for the BLEDevice.
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
/**
    @brief  Constructor
*/
/**************************************************************************/
BlueNRGDevice::BlueNRGDevice(void)
{
    isInitialized = false;
}

/**************************************************************************/
/**
    @brief  Destructor
*/
/**************************************************************************/
BlueNRGDevice::~BlueNRGDevice(void)
{
}


/**
    @brief  Initialises anything required to start using BLE
    @param[in] void
    @returns    ble_error_t
*/
ble_error_t BlueNRGDevice::init(void)
{
    /* ToDo: Clear memory contents, reset the SD, etc. */
    btle_init(BlueNRGGap::getInstance().getIsSetAddress(), D11, D12, D3);

    isInitialized = true;
    
    return BLE_ERROR_NONE;
}


/**
    @brief  Resets the BLE HW, removing any existing services and
            characteristics
    @param[in] void
    @returns    ble_error_t
*/
ble_error_t BlueNRGDevice::reset(void)
{
    wait(0.5);

    /* Reset BlueNRG SPI interface */
    BlueNRG_RST();

    /* Wait for the radio to come back up */
    wait(1);
    
    isInitialized = false;

    return BLE_ERROR_NONE;
}


/*!
    @brief  Wait for any BLE Event like BLE Connection, Read Request etc.    
    @param[in] void
    @returns    char *      
*/
void BlueNRGDevice::waitForEvent(void)
{
    HCI_Process();//Send App Events??
    
}


/*!
    @brief  get GAP version
    @param[in] void
    @returns    char *
*/
const char *BlueNRGDevice::getVersion(void)
{
    char *version = new char[6];
    memcpy((void *)version, "1.0.0", 5);
    return version;
}

/**************************************************************************/
/*!
    @brief  get init state
    @param[in] void
    @returns    bool  
*/
/**************************************************************************/
bool BlueNRGDevice::getIsInitialized(void)
{
    return isInitialized;
}

/**************************************************************************/
/*!
    @brief  get reference to GAP object
    @param[in] void
    @returns    Gap&      
*/
/**************************************************************************/
Gap        &BlueNRGDevice::getGap()        
{
    return BlueNRGGap::getInstance();
}

/**************************************************************************/
/*!
    @brief  get reference to GATT server object
    @param[in] void
    @returns    GattServer&    
*/
/**************************************************************************/
GattServer &BlueNRGDevice::getGattServer() 
{
    return BlueNRGGattServer::getInstance();
}

/**************************************************************************/
/*!
    @brief  set Tx power level
    @param[in] txPower Transmission Power level
    @returns    ble_error_t
*/
/**************************************************************************/
ble_error_t BlueNRGDevice::setTxPower(int8_t txPower)
{
    int8_t enHighPower = 0;
    int8_t paLevel = 0;    
    int8_t dbmActuallySet = getHighPowerAndPALevelValue(txPower, enHighPower, paLevel);
    DEBUG("txPower=%d, dbmActuallySet=%d\n\r", txPower, dbmActuallySet);
    DEBUG("enHighPower=%d, paLevel=%d\n\r", enHighPower, paLevel);                    
    aci_hal_set_tx_power_level(enHighPower, paLevel);
    return BLE_ERROR_NONE;    
}