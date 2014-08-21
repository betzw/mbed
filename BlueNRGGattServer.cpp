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
#include "Utils.h"

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
    
    DEBUG("AddService()\n\r");
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
    GattCharacteristic *p_char = service.getCharacteristic(i);
    uint16_t char_uuid = (p_char->getUUID()).getShortUUID();
    
    uint8_t int_8_uuid[2];
    STORE_LE_16(int_8_uuid, char_uuid);
    //TODO: Check UUID existence??
    DEBUG("Char Properties 0x%x\n\r", p_char->getProperties());
    /*
    * Gatt_Evt_Mask -> HardCoded (0)
    * Encryption_Key_Size -> Hardcoded (16)
    * isVariable (variable length value field) -> Hardcoded (1)
    */
    tGattServerEvent Gatt_Evt_Mask = 0x0;
    
    if((p_char->getProperties() &
         (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE|
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE))) {
            DEBUG("Setting up Gatt GATT_SERVER_ATTR_WRITE Mask\n\r");
            Gatt_Evt_Mask = Gatt_Evt_Mask | GATT_SERVER_ATTR_WRITE;
        }
    if((p_char->getProperties() &
         (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ|
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY))) {
            DEBUG("Setting up Gatt GATT_INTIMATE_APPL_WHEN_READ_N_WAIT Mask\n\r");
            Gatt_Evt_Mask = Gatt_Evt_Mask | GATT_INTIMATE_APPL_WHEN_READ_N_WAIT; 
        }    //This will support also GATT_SERVER_ATTR_READ_WRITE since it will be covered by previous if() check.
        
    ret =  aci_gatt_add_char(service.getHandle(), UUID_TYPE_16, int_8_uuid, p_char->getMaxLength() /*2*/ /*Value Length*/,
                           p_char->getProperties(), ATTR_PERMISSION_NONE, Gatt_Evt_Mask /*Gatt_Evt_Mask*/,
                           16 /*Encryption_Key_Size*/, 1 /*isVariable*/, &bleCharacteristicHandles[characteristicCount]);
    
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
    DEBUG("ReadValue() Not Supported\n\r");
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
  tHalUint8 buff[2];
    
  //STORE_LE_16(buff,125);
  
  //DEBUG("CharHandle: %d\n\r", charHandle);
  //DEBUG("Actual Handle: 0x%x\n\r", bleCharacteristicHandles[charHandle]);
  //DEBUG("Service Handle: 0x%x\n\r", hrmServHandle);
  //DEBUG("buffer[0]: %d\n\r", buffer[0]);
  //DEBUG("buffer[1]: %d\n\r", buffer[1]);
  //DEBUG("len: %d\n\r", len);
    
  ret = aci_gatt_update_char_value(hrmServHandle, bleCharacteristicHandles[charHandle], 0, len, buffer);

  if (ret != BLE_STATUS_SUCCESS){
      DEBUG("Error while updating characteristic.\n\r") ;
      return BLE_ERROR_PARAM_OUT_OF_RANGE ; //Not correct Error Value 
      //FIXME: Define Error values equivalent to BlueNRG Error Codes.
    }

  //Generate Data Sent Event Here? (GattServerEvents::GATT_EVENT_DATA_SENT)  //FIXME: Is this correct?
  //Check if characteristic property is NOTIFY|INDICATE, if yes generate event
  GattCharacteristic *p_char = BlueNRGGattServer::getInstance().getCharacteristicFromHandle(bleCharacteristicHandles[charHandle]);
  if(p_char->getProperties() &  (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
                                  | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE)) {
          BlueNRGGattServer::getInstance().handleEvent(GattServerEvents::GATT_EVENT_DATA_SENT);
      }
  
  return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Reads a value according to the handle provided

    @param[in]  charHandle
                The handle of the GattCharacteristic to read from

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::Read_Request_CB(tHalUint16 handle)
{
    //signed short refvalue;
    uint16_t gapConnectionHandle = BlueNRGGap::getInstance().getConnectionHandle();
    
    tBleStatus ret;
    uint16_t data;
    tHalUint8 buff[2];
        
    data = 450 + ((uint64_t)rand()*100)/RAND_MAX;
    STORE_LE_16(buff,data);
    
    //ret = aci_gatt_update_char_value(hrmServHandle, handle, 0, sizeof(buff), buff);
    //ret = aci_gatt_read_charac_val(gapConnectionHandle, handle);
    
    //EXIT:
    if(gapConnectionHandle != 0)
    aci_gatt_allow_read(gapConnectionHandle);
}

/**************************************************************************/
/*!
    @brief  Returns the GattCharacteristic according to the handle provided

    @param[in]  charHandle
                The handle of the GattCharacteristic

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
GattCharacteristic* BlueNRGGattServer::getCharacteristicFromHandle(tHalUint16 attrHandle)
{
    GattCharacteristic *p_char;
    int i;
    uint16_t handle;
    
    //DEBUG("BlueNRGGattServer::getCharacteristicFromHandle()>>Attribute Handle received 0x%x\n\r",attrHandle);
    for(i=0; i<characteristicCount; i++)
    {
        handle = p_characteristics[i]->getHandle();
        
        if(i==characteristicCount-1)//Last Characteristic check
        {
            if(attrHandle>=bleCharacteristicHandles[handle])
            {
                p_char = p_characteristics[i];
                //DEBUG("Found Characteristic Properties 0x%x\n\r",p_char->getProperties());
                break;
                }            
        }
        else {
            //Testing if attribute handle is between two Characteristic Handles
            if(attrHandle>=bleCharacteristicHandles[handle] && attrHandle<bleCharacteristicHandles[handle+1])
            {
                p_char = p_characteristics[i];
                //DEBUG("Found Characteristic Properties 0x%x\n\r",p_char->getProperties());
                break;
            } else continue;
        }
    }
    return p_char;
}
