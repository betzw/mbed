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
  * @file    BlueNRGDevice.h 
  * @author  STMicroelectronics
  * @brief   Header file for BLEDeviceInstanceBase based BlueNRGDevice
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
  
#ifndef __BLUENRG_H__
#define __BLUENRG_H__

#define BLUENRG
#define DEBUG_BLUENRG_USER

#include "btle.h"

#include "mbed.h"
#include "blecommon.h"
#include "BLEDevice.h"
#include "BlueNRGGap.h"
#include "BlueNRGGattServer.h"


class BlueNRGDevice : public BLEDeviceInstanceBase
{

public:
    BlueNRGDevice(void);
    virtual ~BlueNRGDevice(void);

    virtual Gap        &getGap();
    virtual GattServer &getGattServer();    
    virtual const char *getVersion(void);
    virtual ble_error_t init(void);
    virtual ble_error_t reset(void);
    virtual ble_error_t setTxPower(int8_t txPower);
    virtual void        waitForEvent(void);    

    bool getIsInitialized(void);
    
private:
    bool isInitialized;
};

#endif
