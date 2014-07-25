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

#include "BlueNRGDevice.h"
#include "mbed.h"

//Local Variables
const uint8_t *device_name;

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
ble_error_t BlueNRGGap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* Make sure we have a payload! */
    if (advData.getPayloadLen() == 0) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    //set the advData here in some local variable so that startAdvertising can use it.
    if (advData.getPayloadLen() > 0) {
        const uint8_t *payload = advData.getPayload();
        device_name = advData.getPayload();
    }
    
    
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
ble_error_t BlueNRGGap::startAdvertising(const GapAdvertisingParams &params)
{
    /* Make sure we support the advertising type */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) {
        /* ToDo: This requires a propery security implementation, etc. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Check interval range */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        /* Min delay is slightly longer for unconnectable devices */
        if ((params.getInterval() < GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) ||
            (params.getInterval() > GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    } else {
        if ((params.getInterval() < GAP_ADV_PARAMS_INTERVAL_MIN) ||
            (params.getInterval() > GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }

    /* Check timeout is zero for Connectable Directed */
    if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) && (params.getTimeout() != 0)) {
        /* Timeout must be 0 with this type, although we'll never get here */
        /* since this isn't implemented yet anyway */
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check timeout for other advertising types */
    if ((params.getAdvertisingType() != GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) &&
        (params.getTimeout() > GAP_ADV_PARAMS_TIMEOUT_MAX)) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

  tBleStatus ret;
  
  //const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};
  //const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,device_name[27],device_name[28],device_name[29],device_name[30], device_name[31],
   //                                                     device_name[32], device_name[33], device_name[34], device_name[35], device_name[36]};
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,device_name[13],device_name[14],device_name[15],device_name[16], device_name[17],
                                                       device_name[18], device_name[19], device_name[20], device_name[21], device_name[22]};
  
  //const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,device_name[27],device_name[28]};
  const LongUUID_t HRM_SERVICE_UUID_128 = {0x18, 0x0D};
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  
  /*aci_gap_set_discoverable(Advertising_Event_Type, Adv_min_intvl, Adv_Max_Intvl, Addr_Type, Adv_Filter_Policy,
                        Local_Name_Length, local_name, service_uuid_length, service_uuid_list, Slave_conn_intvl_min, Slave_conn_intvl_max);*/
  /*LINK_LAYER.H DESCRIBES THE ADVERTISING TYPES*/ 
                                
  ret = aci_gap_set_discoverable(params.getAdvertisingType(), params.getInterval(), 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 11 /*Length of the local_name[] array*/, local_name, 0, NULL, 0, 0);
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
ble_error_t BlueNRGGap::stopAdvertising(void)
{
    

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
ble_error_t BlueNRGGap::disconnect(void)
{
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Sets the 16-bit connection handle
*/
/**************************************************************************/
void BlueNRGGap::setConnectionHandle(uint16_t con_handle)
{
    m_connectionHandle = con_handle;
}

/**************************************************************************/
/*!
    @brief  Gets the 16-bit connection handle
*/
/**************************************************************************/
uint16_t BlueNRGGap::getConnectionHandle(void)
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
ble_error_t BlueNRGGap::setAddress(addr_type_t type, const uint8_t address[6])
{
    tBleStatus ret;
    
    if (type > ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
        
    ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, (const tHalUint8*)address);
    
    //if (ret==BLE_STATUS_SUCCESS)
    return BLE_ERROR_NONE;
    
    //return BLE_ERROR_PARAM_OUT_OF_RANGE;
}
