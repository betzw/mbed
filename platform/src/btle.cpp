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
  * @file    btle.cpp 
  * @author  STMicroelectronics
  * @brief   Implementation BlueNRG Init and helper functions.
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


#include "btle.h"
#include "ble/Gap.h"
#include "ble/GapEvents.h"
#include "BlueNRGGap.h"
#include "BlueNRGGattServer.h"
#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif


/* C File Includes ------------------------------------------------------------------*/
// ANDREA: Updated includes and changed some types (e.g., tHalUint8 --> uint8_t)
#include <stdio.h>
#include <string.h>
#include "hci.h"
#include "hci_const.h"
#include "bluenrg_aci.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_gap.h"

#include "hal_types.h"
#include "hal.h"
#include "gp_timer.h"
#include "osal.h"
#include "sm.h"
#include "debug.h"

#ifdef __cplusplus
}
#endif


static void btle_handler(/*ble_evt_t * p_ble_evt*/);
void HCI_Input(tHciDataPacket * hciReadPacket);

//#define BDADDR_SIZE 6
//tHalUint8 bdaddr[BDADDR_SIZE]= {0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02};

uint16_t g_gap_service_handle = 0;
uint16_t g_appearance_char_handle = 0;
uint16_t g_device_name_char_handle = 0;

/* Private variables ---------------------------------------------------------*/
volatile uint8_t set_connectable = 1;
// ANDREA
Gap::Address_t bleAddr;
Gap::AddressType_t addr_type = Gap::ADDR_TYPE_PUBLIC;

/**************************************************************************/
/*!
    @brief  Initialises BTLE and the underlying HW/Device
    @param  isSetAddress boolean if address has been set
    @param  mosi MOSI Pin
    @param  miso MISO Pin
    @param  sclk clock Pin
    @returns void
*/
/**************************************************************************/
void btle_init(bool isSetAddress)
{
    DEBUG("btle_init>>\n\r"); 
    
    int ret;
    uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

    /* Delay needed only to be able to acces the JTAG interface after reset
    if it will be disabled later. */
    Clock_Wait(500);

    /* Initialize the BlueNRG HCI */
    HCI_Init();

    /* Reset BlueNRG SPI interface */
    BlueNRG_RST();

    /* The Nucleo board must be configured as SERVER */
    //check if isSetAddress is set than set address.
    // ANDREA
    if(isSetAddress)
    {
        BlueNRGGap::getInstance().getAddress(&addr_type, bleAddr);
        
        Gap::Address_t bdaddr;
        Osal_MemCpy(bdaddr, bleAddr, BDADDR_SIZE);

        ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                        CONFIG_DATA_PUBADDR_LEN,
                                        bdaddr);
    } else {
        
        const Gap::Address_t BLE_address_BE = {0xFD,0x00,0x25,0xAA,0x02,0x04};
        BlueNRGGap::getInstance().setAddress(Gap::ADDR_TYPE_PUBLIC, BLE_address_BE);
        
        ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                        CONFIG_DATA_PUBADDR_LEN,
                                        BLE_address_BE);
    }
    
    ret = aci_gatt_init();
    if(ret){
        PRINTF("GATT_Init failed.\n");
    }
    //GAP is always in PERIPHERAL _ROLE as mbed does not support Master role at the moment
#ifdef BLUENRG_MS
    ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
#else
    ret = aci_gap_init(GAP_PERIPHERAL_ROLE, &service_handle, &dev_name_char_handle, &appearance_char_handle);
#endif
    if(ret != BLE_STATUS_SUCCESS){
        PRINTF("GAP_Init failed.\n");
    }

    //ANDREA -- FIXME: Security and passkey set by default    
    ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                                       OOB_AUTH_DATA_ABSENT,
                                       NULL,
                                       7,
                                       16,
                                       USE_FIXED_PIN_FOR_PAIRING,
                                       123456,
                                       BONDING);
    if (ret == BLE_STATUS_SUCCESS) {
        DEBUG("Auth Req set successfully.\n");
    }
    
    aci_hal_set_tx_power_level(1,4);
    
    g_gap_service_handle = service_handle;
    g_appearance_char_handle = appearance_char_handle;
    g_device_name_char_handle = dev_name_char_handle; 
    //Device Name is set from Accumulate Adv Data Payload or through setDeviceName API  
    /*ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
                            strlen(name), (tHalUint8 *)name);*/
    
    return;
}

void User_Process()
{
    if(set_connectable){
        setConnectable();
        set_connectable = FALSE;
    }
}

void setConnectable(void)
{  
    tBleStatus ret;

    const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};

    /* disable scan response */
    hci_le_set_scan_resp_data(0,NULL);

    //int t = BlueNRGGap::getInstance()::ADV_IND;// advType;
    
    ret = aci_gap_set_discoverable(BlueNRGGap::getInstance().ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
    sizeof(local_name), local_name, 0, NULL, 0, 0);
    if (ret != BLE_STATUS_SUCCESS) {
        DEBUG("Error while setting discoverable mode (%d)\n", ret);    
    }

}

/**************************************************************************/
/*!
    @brief  Not Used

    @param[in]  void
    
    @returns
*/
/**************************************************************************/
static void btle_handler()
{

}

/*!
    @brief  Not Used

    @param[in]  void
    
    @returns
*/
void SPI_Poll(void)
{
    //HAL_GPIO_EXTI_Callback_Poll(BNRG_SPI_EXTI_PIN);
    return;
}

#ifdef __cplusplus
extern "C" {
#endif

    /**************************************************************************/
    /*!
    @brief      Handle HCI Stack Event

    @param[in]  pckt
                Event Packet sent by the stack to be decoded
    
    @returns
    */
    /**************************************************************************/
    extern void HCI_Event_CB(void *pckt) {
        
        hci_uart_pckt *hci_pckt = (hci_uart_pckt*)pckt;
        hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
        
        if(hci_pckt->type != HCI_EVENT_PKT)
        return;

        switch(event_pckt->evt){
            
        case EVT_DISCONN_COMPLETE:
            {
                DEBUG("EVT_DISCONN_COMPLETE\n");
                
                evt_disconn_complete *evt = (evt_disconn_complete*)event_pckt;
                
                BlueNRGGap::getInstance().processDisconnectionEvent(evt->handle, BlueNRGGap::REMOTE_USER_TERMINATED_CONNECTION);
            }
            break;
            
        case EVT_LE_META_EVENT:
            {
                DEBUG("EVT_LE_META_EVENT\n");
                
                evt_le_meta_event *evt = (evt_le_meta_event *)event_pckt->data;
                
                switch(evt->subevent){
                // ANDREA
                case EVT_LE_CONN_COMPLETE:
                    {                            
                        Gap::AddressType_t peerAddrType = Gap::ADDR_TYPE_PUBLIC;
                        DEBUG("EVT_LE_CONN_COMPLETE\n");
                        
                        evt_le_connection_complete *cc = (evt_le_connection_complete *)evt->data;
                        
                        BlueNRGGap::getInstance().setConnectionHandle(cc->handle);
                        BlueNRGGap::ConnectionParams_t connectionParams;
                        BlueNRGGap::getInstance().getPreferredConnectionParams(&connectionParams);
                        switch (cc->peer_bdaddr_type) {
                            case PUBLIC_ADDR:
                                peerAddrType = Gap::ADDR_TYPE_PUBLIC;
                                break;
                            case STATIC_RANDOM_ADDR:
                                peerAddrType = Gap::ADDR_TYPE_RANDOM_STATIC;
                                break;
                            case RESOLVABLE_PRIVATE_ADDR:
                                peerAddrType = Gap::ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE;
                                break;
                            case NON_RESOLVABLE_PRIVATE_ADDR:
                                peerAddrType = Gap::ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE;
                                break;
                        }                                             
                        BlueNRGGap::getInstance().processConnectionEvent(cc->handle, Gap::PERIPHERAL, peerAddrType, cc->peer_bdaddr, addr_type, bleAddr, (const BlueNRGGap::ConnectionParams_t *)&connectionParams);                            
                    }
                    break;
                }
            }
            break;
            
        case EVT_VENDOR:
            {                
                evt_blue_aci *blue_evt = (evt_blue_aci*)event_pckt->data;
                //DEBUG("EVT_VENDOR %d\n", blue_evt->ecode);
                
                switch(blue_evt->ecode){
                           
                case EVT_BLUE_GATT_READ_PERMIT_REQ:
                    {
                        DEBUG("EVT_BLUE_GATT_READ_PERMIT_REQ_OK\n\r");
                        evt_gatt_read_permit_req *pr = (evt_gatt_read_permit_req*)blue_evt->data;                        
                        BlueNRGGattServer::getInstance().Read_Request_CB(pr->attr_handle);                                                
                    }
                    break;
                    
                case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:         
                    {
                        /* this callback is invoked when a GATT attribute is modified
                            extract callback data and pass to suitable handler function */
                        evt_gatt_attr_modified *evt = (evt_gatt_attr_modified*)blue_evt->data;
                        DEBUG("EVT_BLUE_GATT_ATTRIBUTE_MODIFIED\n\r");
                        
                        //DEBUG("CharHandle 0x%x, length: 0x%x, Data: 0x%x\n\r",evt->attr_handle, evt->data_length, (uint16_t)evt->att_data[0]);
                        DEBUG("CharHandle %d, length: %d, Data: %d\n\r",evt->attr_handle, evt->data_length, (uint16_t)evt->att_data[0]);       
                        
                        //Extract the GattCharacteristic from p_characteristics[] and find the properties mask
                        GattCharacteristic *p_char = BlueNRGGattServer::getInstance().getCharacteristicFromHandle(evt->attr_handle);
                        if(p_char!=NULL) {
                            DEBUG("getProperties 0x%x\n\r",p_char->getProperties());
                            if((p_char->getProperties() &  (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
                                            | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE))) {
                                
                                //Now Check if data written in Enable or Disable
                                if((uint16_t)evt->att_data[0]==1) {
                                    //DEBUG("Notify ENABLED\n\r"); 
                                    BlueNRGGattServer::getInstance().HCIEvent(GattServerEvents::GATT_EVENT_UPDATES_ENABLED, evt->attr_handle);
                                } 
                                else {
                                    //DEBUG("Notify DISABLED\n\r"); 
                                    BlueNRGGattServer::getInstance().HCIEvent(GattServerEvents::GATT_EVENT_UPDATES_DISABLED, evt->attr_handle);
                                }
                            }
                            
                            //Check is attr handle property is WRITEABLE, if yes, generate GATT_EVENT_DATA_WRITTEN Event
                            if((p_char->getProperties() &
                                        (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE|
                                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE))) {
                                
                                GattWriteCallbackParams writeParams;
                                writeParams.handle=evt->attr_handle;
                                writeParams.writeOp=GattWriteCallbackParams::OP_WRITE_REQ;//Where to find this property in BLUENRG?
                                writeParams.len=evt->data_length;
                                writeParams.data=evt->att_data;                                                                                    
                                #ifdef BLUENRG_MS
                                writeParams.offset=evt->offset;//Not used in BlueNRG?
                                #endif
                                BlueNRGGattServer::getInstance().HCIDataWrittenEvent(&writeParams);

                                //BlueNRGGattServer::getInstance().handleEvent(GattServerEvents::GATT_EVENT_DATA_WRITTEN, evt->attr_handle);
                                //Write the actual Data to the Attr Handle? (uint8_1[])evt->att_data contains the data
                                if ((p_char->getValueAttribute().getValuePtr() != NULL) && (p_char->getValueAttribute().getInitialLength() > 0)) {
                                    BlueNRGGattServer::getInstance().write(p_char->getValueAttribute().getHandle(),
                                                                            p_char->getValueAttribute().getValuePtr(),
                                                                            p_char->getValueAttribute().getInitialLength(),
                                                                            false /* localOnly */);
                                }
                            } 
                        }                  
                    }
                    break;  
                    
                    //Any cases for Data Sent Notifications?
                case EVT_BLUE_GATT_NOTIFICATION:
                    //This is only relevant for Client Side Event
                    DEBUG("EVT_BLUE_GATT_NOTIFICATION");
                    break;
                case EVT_BLUE_GATT_INDICATION:
                    //This is only relevant for Client Side Event
                    DEBUG("EVT_BLUE_GATT_INDICATION");
                    break;   
                    
                case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
                    DEBUG("EVT_BLUE_GATT_PROCEDURE_COMPLETE");
                    break;                                     
                }
            }
            break;
        }    
        
        return ;
    }


#ifdef __cplusplus
}
#endif