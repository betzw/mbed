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

#include "BlueNRGGattServer.h"
#include "mbed.h"
#include "BlueNRGGap.h"

#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (tHalUint8) (val)    ) , \
                                   ((buf)[1] =  (tHalUint8) (val>>8) ) )

#define STORE_LE_32(buf, val)    ( ((buf)[0] =  (tHalUint8) (val)     ) , \
                                   ((buf)[1] =  (tHalUint8) (val>>8)  ) , \
                                   ((buf)[2] =  (tHalUint8) (val>>16) ) , \
                                   ((buf)[3] =  (tHalUint8) (val>>24) ) ) 
                          
/**************************************************************************/
/*!
    @brief  Adds a new service to the GATT table on the peripheral

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::addService(GattService &service)
{
    /* ToDo: Make sure we don't overflow the array, etc. */
    /* ToDo: Make sure this service UUID doesn't already exist (?) */
    /* ToDo: Basic validation */
    
    tBleStatus ret;
    
    
    /* Add the service to the BlueNRG */
    uint16_t short_uuid = (service.getUUID()).getShortUUID();
    
    uint8_t primary_uuid[2];//= {0x0D,0x18};    
    STORE_LE_16(primary_uuid, short_uuid);
    
    //TODO: Check UUID existence??
    
    ret = aci_gatt_add_serv(UUID_TYPE_16, primary_uuid, PRIMARY_SERVICE, 7, 
                            &hrmServHandle);
    service.setHandle(hrmServHandle);
    
    //TODO: iterate to include all characteristics
    for (uint8_t i = 0; i < service.getCharacteristicCount(); i++) {
    GattCharacteristic *p_char = service.getCharacteristic(0);
    uint16_t char_uuid = (p_char->getUUID()).getShortUUID();
    
    //TODO: Check UUID existence??
    
    ret =  aci_gatt_add_char(service.getHandle(), UUID_TYPE_16, (const uint8_t*)char_uuid, 1,
                           p_char->getProperties()/*CHAR_PROP_NOTIFY*/, ATTR_PERMISSION_NONE, 0,
                           16, 0, /*&hrmCharHandle*/ &bleCharacteristicHandles[characteristicCount]);
    
    /* Update the characteristic handle */
    uint16_t charHandle = characteristicCount;    
    
    p_characteristics[characteristicCount++] = p_char;
    p_char->setHandle(charHandle);    //Set the characteristic count as the corresponding char handle
    
    if ((p_char->getValuePtr() != NULL) && (p_char->getInitialLength() > 0)) {
            updateValue(charHandle, p_char->getValuePtr(), p_char->getInitialLength(), false /* localOnly */);
        }
    }    
                       
    serviceCount++;
    
    //FIXME: There is no GattService pointer array in GattServer. 
    //        There should be one? (Only the user is aware of GattServices!) Report to forum.
                    
    return BLE_ERROR_NONE;
}


/**************************************************************************/
/*!
    @brief  Reads the value of a characteristic, based on the service
            and characteristic index fields

    @param[in]  charHandle
                The handle of the GattCharacteristic to read from
    @param[in]  buffer
                Buffer to hold the the characteristic's value
                (raw byte array in LSB format)
    @param[in]  len
                The number of bytes read into the buffer

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::readValue(uint16_t charHandle, uint8_t buffer[], uint16_t *const lengthP)
{
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Updates the value of a characteristic, based on the service
            and characteristic index fields

    @param[in]  charHandle
                The handle of the GattCharacteristic to write to
    @param[in]  buffer
                Data to use when updating the characteristic's value
                (raw byte array in LSB format)
    @param[in]  len
                The number of bytes in buffer

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::updateValue(uint16_t charHandle, uint8_t buffer[], uint16_t len, bool localOnly)
{
    tBleStatus ret;    
  tHalUint8 buff[6];
    
  STORE_LE_16(buff,125);
  STORE_LE_16(buff+2,145);
  STORE_LE_16(buff+4,543);
    
  ret = aci_gatt_update_char_value(hrmServHandle, charHandle, 0, 6, buff);

    return BLE_ERROR_NONE;
}
