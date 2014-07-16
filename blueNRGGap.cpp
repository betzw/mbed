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
  
#include "blueNRGGap.h"
#include "mbed.h"


/**************************************************************************/
/*!
    @brief  Sets the advertising parameters and payload for the device

    @param[in]  params
                Basic advertising details, including the advertising
                delay, timeout and how the device should be advertised
    @params[in] advData
                The primary advertising data payload
    @params[in] scanResponse
                The optional Scan Response payload if the advertising
                type is set to \ref GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED
                in \ref GapAdveritinngParams
            
    @returns    \ref ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly

    @retval     BLE_ERROR_BUFFER_OVERFLOW
                The proposed action would cause a buffer overflow.  All
                advertising payloads must be <= 31 bytes, for example.
                
    @retval     BLE_ERROR_NOT_IMPLEMENTED
                A feature was requested that is not yet supported in the
                nRF51 firmware or hardware.

    @retval     BLE_ERROR_PARAM_OUT_OF_RANGE
                One of the proposed values is outside the valid range.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t blueNRGGap::setAdvertisingData(const GapAdvertisingData & advData, const GapAdvertisingData & scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
   
    /* Make sure we have a payload! */
    
    /* Check the scan response payload limits */
    
    
    
    /* Send advertising data! */
    
    /* Make sure the GAP Service appearance value is aligned with the appearance from GapAdvertisingData */ 
 
    /* ToDo: Perform some checks on the payload, for example the Scan Response can't */
    /* contains a flags AD type, etc. */

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Starts the BLE HW, initialising any services that were
            added before this function was called.
            
    @note   All services must be added before calling this function!
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t blueNRGGap::startAdvertising(const GapAdvertisingParams & params)
{
      /* Make sure we support the advertising type */
    

    /* Check interval range */
    
    

    /* Check timeout is zero for Connectable Directed */
    
    /* Check timeout for other advertising types */
    

    /* Start Advertising */    
        
    state.advertising = 1;

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Stops the BLE HW and disconnects from any devices
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t blueNRGGap::stopAdvertising(void)
{
  state.advertising = 0;

  return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Disconnects if we are connected to a central device
            
    @returns    ble_error_t
    
    @retval     BLE_ERROR_NONE
                Everything executed properly
                
    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t blueNRGGap::disconnect(void)
{
    state.advertising = 0;
  state.connected = 0;
    
    
  return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Sets the 16-bit connection handle
*/
/**************************************************************************/
void blueNRGGap::setConnectionHandle(uint16_t con_handle)
{
  m_connectionHandle = con_handle;
}
 
/**************************************************************************/
/*!
    @brief  Gets the 16-bit connection handle
*/
/**************************************************************************/
uint16_t blueNRGGap::getConnectionHandle(void)
{
  return m_connectionHandle;
}
 
/**************************************************************************/
/*!
    @brief      Sets the BLE device address
            
    @returns    ble_error_t

    @section EXAMPLE

    @code

    uint8_t device_address[6] = { 0xca, 0xfe, 0xf0, 0xf0, 0xf0, 0xf0 };
    nrf.getGap().setAddress(Gap::ADDR_TYPE_RANDOM_STATIC, device_address);

    @endcode
*/
/**************************************************************************/
ble_error_t blueNRGGap::setAddress(addr_type_t type, const uint8_t address[6])
{
  
  return BLE_ERROR_NONE;
}
