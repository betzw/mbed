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
#include "Payload.h"
#include "Utils.h"

//Local Variables
const char *local_name = NULL;
uint8_t local_name_length = 0;

uint8_t servUuidlength = 0;
uint8_t* servUuidData = NULL;

uint32_t advtInterval = 0;

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
    if (advData.getPayloadLen() <= 0) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    } else { //set the advData here in some local variable so that startAdvertising can use it.
        Payload load(advData.getPayload(), advData.getPayloadLen());
        
        for(uint8_t index=0; index<load.getPayloadUnitCount(); index++) {
            //UnitPayload unitLoad = load.getPayLoadAtIndex(index);
            switch(load.getIDAtIndex(index)) {
                case GapAdvertisingData::FLAGS:                              /* ref *Flags */
                break;
                case GapAdvertisingData::INCOMPLETE_LIST_16BIT_SERVICE_IDS:  /**< Incomplete list of 16-bit Service IDs */
                break;
                case GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS:    /**< Complete list of 16-bit Service IDs */
                    DEBUG("Advertising type: COMPLETE_LIST_16BIT_SERVICE_IDS\n");
                    servUuidlength = load.getLengthAtIndex(index)-1;
                    servUuidData = load.getDataAtIndex(index);
                    DEBUG("servUuidlength=%d\n", servUuidlength);
                    break;
                case GapAdvertisingData::INCOMPLETE_LIST_32BIT_SERVICE_IDS:  /**< Incomplete list of 32-bit Service IDs (not relevant for Bluetooth 4.0) */
                break;
                case GapAdvertisingData::COMPLETE_LIST_32BIT_SERVICE_IDS:    /**< Complete list of 32-bit Service IDs (not relevant for Bluetooth 4.0) */
                break;
                case GapAdvertisingData::INCOMPLETE_LIST_128BIT_SERVICE_IDS: /**< Incomplete list of 128-bit Service IDs */
                break;
                case GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS:   /**< Complete list of 128-bit Service IDs */
                break;
                case GapAdvertisingData::SHORTENED_LOCAL_NAME:               /**< Shortened Local Name */
                break;
                case GapAdvertisingData::COMPLETE_LOCAL_NAME:                /**< Complete Local Name */
                    DEBUG("Advertising type: COMPLETE_LOCAL_NAME\n");
                    const char *device_name = NULL;
                    device_name = (const char*)load.getDataAtIndex(index);  // to be set later when startAdvertising() is called
                    DEBUG("input: device_name= %s...\n", device_name);
                    DEBUG("input: device_name length= %d...\n", load.getLengthAtIndex(index)-1);                                              
                    char *namePtr = new char[load.getLengthAtIndex(index)];
                    namePtr[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
                    strcpy(namePtr+1, device_name);
                    local_name_length = load.getLengthAtIndex(index)-1;                        
                    local_name = (const char*)namePtr;  
                    // also set device name in GAP service 
                    aci_gatt_update_char_value(g_gap_service_handle, g_device_name_char_handle, 0, local_name_length, (tHalUint8 *)local_name);
                    
                    DEBUG("setting name to: %s...\n", namePtr+1);
                    DEBUG("name string length: %d...\n", local_name_length+1);    // This includes 'AD_TYPE_COMPLETE_LOCAL_NAME' byte                
                    DEBUG("device_name length=%d", local_name_length);                                    
                    break;
                
                case GapAdvertisingData::TX_POWER_LEVEL:                     /**< TX Power Level (in dBm) */
                    DEBUG("Advertising type: TX_POWER_LEVEL\n");
                    int8_t dbm = load.getInt8AtIndex(index);
                    int8_t enHighPower = 0;
                    int8_t paLevel = 0;
                    int8_t dbmActuallySet = getHighPowerAndPALevelValue(dbm, enHighPower, paLevel);
                    DEBUG("dbm=%d, dbmActuallySet=%d\n", dbm, dbmActuallySet);
                    DEBUG("enHighPower=%d, paLevel=%d\n", enHighPower, paLevel);
                    aci_hal_set_tx_power_level(enHighPower, paLevel);
                    break;
                case GapAdvertisingData::DEVICE_ID:                          /**< Device ID */
                break;
                case GapAdvertisingData::SLAVE_CONNECTION_INTERVAL_RANGE:    /**< Slave :Connection Interval Range */
                break;
                case GapAdvertisingData::SERVICE_DATA:                       /**< Service Data */
                break;
                case GapAdvertisingData::APPEARANCE:   
                    /* 
                    Tested with GapAdvertisingData::GENERIC_PHONE. 
                    for other appearances BLE Scanner android app is not behaving properly 
                    */
                    DEBUG("Advertising type: APPEARANCE\n");
                    const char *deviceAppearance = NULL;
                    deviceAppearance = (const char*)load.getDataAtIndex(index);  // to be set later when startAdvertising() is called
                    DEBUG("input: deviceAppearance= 0x%x 0x%x..., strlen(deviceAppearance)=%d\n", deviceAppearance[1], deviceAppearance[0], load.getLengthAtIndex(index)-1);         /**< \ref Appearance */
                    aci_gatt_update_char_value(g_gap_service_handle, g_appearance_char_handle, 0, 2, (tHalUint8 *)deviceAppearance);
                    break;
                case GapAdvertisingData::ADVERTISING_INTERVAL:               /**< Advertising Interval */
                // taken care of in startAdvertising(params)
                    advtInterval = load.getUint16AtIndex(index);
                    DEBUG("advtInterval=%d\n", advtInterval);
                break;
                case GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA:        /**< Manufacturer Specific Data */                                
                break;
                                
            }
    }
        //const uint8_t *payload = advData.getPayload();

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
  

  
  //const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,device_name[27],device_name[28]};
  const LongUUID_t HRM_SERVICE_UUID_128 = {0x18, 0x0D};
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  
  /*aci_gap_set_discoverable(Advertising_Event_Type, Adv_min_intvl, Adv_Max_Intvl, Addr_Type, Adv_Filter_Policy,
                        Local_Name_Length, local_name, service_uuid_length, service_uuid_list, Slave_conn_intvl_min, Slave_conn_intvl_max);*/
  /*LINK_LAYER.H DESCRIBES THE ADVERTISING TYPES*/ 
                                
  advtInterval = params.getInterval(); // set advtInterval in case it is not already set by user application    
  ret = aci_gap_set_discoverable(params.getAdvertisingType(), // Advertising_Event_Type                                
                                0,   // Adv_Interval_Min
                                advtInterval,   // Adv_Interval_Max
                                RANDOM_ADDR, // Address_Type <hdd> It seems there is some problem with RANDOM_ADDRESS. <problem_desc> When RANDOM_ADDRESS is selected device name is not being handled properly, i.e. wrong device name is seen by android app </problem_desc>
                                NO_WHITE_LIST_USE,  // Adv_Filter_Policy
                                local_name_length, // Local_Name_Length
                                local_name, // Local_Name
                                servUuidlength,  //Service_Uuid_Length
                                servUuidData, //Service_Uuid_List
                                0, // Slave_Conn_Interval_Min
                                0);  // Slave_Conn_Interval_Max
                       
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
