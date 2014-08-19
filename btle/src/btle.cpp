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


#include "btle.h"
#include "hw/Gap.h"
#include "hw/GapEvents.h"
#include "BlueNRGGap.h"
#include "BlueNRGGattServer.h"
#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* C File Includes ------------------------------------------------------------------*/
#include "hal_types.h"
#include "hci.h"
#include "bluenrg_hci.h"
#include "gp_timer.h"
#include "hal.h"
#include "osal.h"
#include "hci_internal.h"
#include "bluenrg_hci_internal.h"
#include "gap.h"
#include "sm.h"
#include <stdio.h>
#include <string.h>
#include <stm32l0xx_bluenrg_shield_bsp.h>
#include "role_type.h"
#include "debug.h"

/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

#ifdef __cplusplus
    }
#endif


static void btle_handler(/*ble_evt_t * p_ble_evt*/);
void HCI_Input(tHciDataPacket * hciReadPacket);

#define BDADDR_SIZE 6
tHalUint8 bdaddr[BDADDR_SIZE]= {0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02};

uint16_t g_gap_service_handle = 0;
uint16_t g_appearance_char_handle = 0;
uint16_t g_device_name_char_handle = 0;

/* Private variables ---------------------------------------------------------*/
volatile uint8_t set_connectable = 1;

/**************************************************************************/
/*!
    @brief      Initialises BTLE and the underlying HW/Device

    @returns
*/
/**************************************************************************/
void btle_init(void)
{
  const char *name = "BlueNRG_1";
  tHalUint8 SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x02};//MPD: This is the public address of the Server/Client
  tHalUint8 bdaddr[BDADDR_SIZE];
  int ret;
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  
  HAL_Init();
  
  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  /* Configure the system clock */
  SystemClock_Config();

  /* Delay needed only to be able to acces the JTAG interface after reset
    if it will be disabled later. */
  Clock_Wait(500);
  
  /* Initialize the BlueNRG SPI driver */
  BNRG_SPI_Init();
  
  /* Initialize the BlueNRG HCI */
  HCI_Init();
  
  /* Reset BlueNRG SPI interface */
  BlueNRG_RST();

  /* The Nucleo board must be configured as SERVER */
  /*Osal_MemCpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));
  
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);*/
                                  
  ret = aci_gatt_init();
  //GAP is always in PERIPHERAL _ROLE as mbed does not support Master role at the moment
  ret = aci_gap_init(GAP_PERIPHERAL_ROLE, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  
  g_gap_service_handle = service_handle;
  g_appearance_char_handle = appearance_char_handle;
  g_device_name_char_handle = dev_name_char_handle;   
  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
                               strlen(name), (tHalUint8 *)name);
                               
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
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G', '1', '2'};
  
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  
  
  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 8, local_name, 0, NULL, 0, 0);
  
}

/**************************************************************************/
/*!
    @brief

    @param[in]  p_ble_evt
    
    @returns
*/
/**************************************************************************/
static void btle_handler()
{
  
}


#ifdef __cplusplus
extern "C" {
#endif

extern void HCI_Event_CB(void *pckt) {
    
    hci_uart_pckt *hci_pckt = (hci_uart_pckt*)pckt;
    hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
    
    if(hci_pckt->type != HCI_EVENT_PKT)
    return;

    switch(event_pckt->evt){
        
        case EVT_DISCONN_COMPLETE:
            {
              BlueNRGGap::getInstance().handleEvent(GapEvents::GAP_EVENT_DISCONNECTED);
            }
            break;
        
        case EVT_LE_META_EVENT:
            {
                evt_le_meta_event *evt = (evt_le_meta_event *)event_pckt->data;
              
                switch(evt->subevent){
                    case EVT_LE_CONN_COMPLETE:
                        {                            
                            evt_le_connection_complete *cc = (evt_le_connection_complete *)evt->data;
                            
                            BlueNRGGap::getInstance().setConnectionHandle(cc->handle);
                            BlueNRGGap::getInstance().handleEvent(GapEvents::GAP_EVENT_CONNECTED);
                            
                        }
                        break;
                    }
            }
            break;
        
        case EVT_VENDOR:
            {
                evt_blue_aci *blue_evt = (evt_blue_aci*)event_pckt->data;
                switch(blue_evt->ecode){
    
                    case EVT_BLUE_GATT_READ_PERMIT_REQ:
                        {
                            DEBUG("EVT_BLUE_GATT_READ_PERMIT_REQ\n\r");
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
                          DEBUG("CharHandle 0x%x, Data: 0x%x\n\r",evt->attr_handle, evt->att_data);
                          
                          BlueNRGGattServer::getInstance().handleEvent(GattServerEvents::GATT_EVENT_DATA_WRITTEN, evt->attr_handle);
                          
                          //Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data);                    
                        }
                        break;  
                        
                    //Any cases for Data Sent Notifications?
                    case EVT_BLUE_GATT_NOTIFICATION:
                    DEBUG("EVT_BLUE_GATT_NOTIFICATION");
                    break;
                    case EVT_BLUE_GATT_INDICATION:
                    DEBUG("EVT_BLUE_GATT_INDICATION");
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