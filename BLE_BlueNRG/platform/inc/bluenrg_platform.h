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

#ifndef __BLUENRG_PLATFORM_H_
#define __BLUENRG_PLATFORM_H_

#include "pinmap.h"

// SPI Instance
#define BNRG_SPI_INSTANCE   0x1234

/* Platform Pin Configurations -----------------------------------------------*/
#define MBED_NUCLEO_PLATFORM
//#define MBED_FRDM_PLATFORM
//#define MBED_LPC_PLATFORM

#ifdef MBED_FRDM_PLATFORM
PinName PIN_SPI_MOSI=PTD2;//D11
PinName PIN_SPI_MISO=PTD3;//D12
PinName PIN_SPI_CLK=PTD1;//D13
PinName PIN_SPI_CS=PTD0;//D10
PinName PIN_SPI_RESET=PTC3;//D7
PinName PIN_SPI_IRQ=PTB2;//A0
#endif

#ifdef MBED_LPC_PLATFORM
PinName PIN_SPI_MOSI=P0_9;//D11
PinName PIN_SPI_MISO=P0_8;//D12
PinName PIN_SPI_CLK=P1_29;//D13
PinName PIN_SPI_CS=P1_28;//D8
PinName PIN_SPI_RESET=P1_25;//D7
PinName PIN_SPI_IRQ=P1_9;//A0
#endif

#ifdef MBED_NUCLEO_PLATFORM
PinName PIN_SPI_MOSI=PA_7;//D12
PinName PIN_SPI_MISO=PA_6;//D11
PinName PIN_SPI_CLK=PB_3;//D3
PinName PIN_SPI_CS=PA_1;//A1
PinName PIN_SPI_RESET=PA_8;//D7
PinName PIN_SPI_IRQ=PA_0;//A0
#endif

#endif //__PLATFORM_H_
