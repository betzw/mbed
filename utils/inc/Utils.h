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
 
 #include "hal_types.h"
 #include "mbed.h"
 
 // utility functions
 
#ifndef __UTIL_H__
#define __UTIL_H__

#define NEED_CONSOLE_OUTPUT 1 /* Set this if you need debug messages on the console;
                               * it will have an impact on code-size and power consumption. */
 
#if NEED_CONSOLE_OUTPUT
//Serial usb(USBTX, USBRX); // tx, rx
extern Serial  pc;
#define DEBUG(...) { pc.printf(__VA_ARGS__); }
#else
#define DEBUG(...) /* nothing */
#endif /* #if NEED_CONSOLE_OUTPUT */

#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (tHalUint8) (val)    ) , \
                                   ((buf)[1] =  (tHalUint8) (val>>8) ) )

#define STORE_LE_32(buf, val)    ( ((buf)[0] =  (tHalUint8) (val)     ) , \
                                   ((buf)[1] =  (tHalUint8) (val>>8)  ) , \
                                   ((buf)[2] =  (tHalUint8) (val>>16) ) , \
                                   ((buf)[3] =  (tHalUint8) (val>>24) ) ) 


double getHighPowerAndPALevelValue(int8_t dBMLevel, int8_t& EN_HIGH_POWER, int8_t& PA_LEVEL);

#endif // __UTIL_H__