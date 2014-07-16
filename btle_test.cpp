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
/*
#ifdef __cplusplus
extern "C" {
#endif*/
#if 0
#include "btle.h"

#include "hw/Gap.h"
#include "hw/GattServer.h"
#include "hw/BLEDevice.h"

/*
#ifdef __cplusplus
    }
#endif*/
#include "mbed.h"
#include "blecommon.h"

#include "blueNRGGap.h"
#include "blueNRGGattServer.h"


/*************************************************************************
!
    \brief


*************************************************************************/
void test_function(void);

void test_function(void)
{
    //btle_init();
    
    }

class BTLE_Test //: public BLEDeviceInstanceBase
{
    public:   
        BTLE_Test(void);
        virtual ~BTLE_Test(void); 
        void Init(void);
        
        /*virtual Gap&        getGap()        { return blueNRGGap::getInstance(); };
        virtual GattServer& getGattServer() { return blueNRGGattServer::getInstance(); };
        virtual ble_error_t init(void);
        virtual ble_error_t reset(void);
        virtual void waitForEvent(void);      */
    };

void BTLE_Test::Init(void) {
    //btle_init();
    }

#endif    