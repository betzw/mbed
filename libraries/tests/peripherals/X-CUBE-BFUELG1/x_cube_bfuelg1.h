/**
  ******************************************************************************
  * @file    x_cube_bfuelg1.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    08-October-2014
  * @brief   Header file for class X_CUBE_BFUELG1 representing an X-CUBE-BFUELG1
  *          expansion board
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __X_CUBE_BFUELG1_H
#define __X_CUBE_BFUELG1_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_cube_bfuelg1_targets.h"
#include "x_cube_bfuelg1_charger.h"
#include "x_cube_bfuelg1_rtc.h"
#include "x_cube_bfuelg1_i2c.h"
#include "Components/Common/GasGauge.h"

/* Classes -------------------------------------------------------------------*/
/** Class X_CUBE_BFUELG1 is intended to represent the battery management
 *  expansion board with the same name.
 *
 *  The expansion board is featuring basically three IPs:\n
 *  -# a battery charger of class L6924D\n
 *  -# a real-time clock (RTC) of class M41T62\n
 *  -# and a gas gauge (GG) of class STC3115\n
 *
 * It is intentionally implemented as a singleton because only one
 * X_CUBE_BFUELG1 at a time might be deployed in a HW component stack.\n
 * In order to get the singleton instance you have to call class method `Instance()`, 
 * e.g.:
 * @code
 * // Battery expansion board singleton instance
 * static X_CUBE_BFUELG1 *battery_expansion_board = X_CUBE_BFUELG1::Instance();
 * @endcode
 */
class X_CUBE_BFUELG1
{
protected:
    X_CUBE_BFUELG1(void);

public:
    static X_CUBE_BFUELG1* Instance(void);

    DevI2C dev_i2c;

    L6924D charger;
    M41T62 rtc;
    GasGauge &gg;

private:
    static X_CUBE_BFUELG1 *_instance;
};

#endif /* __X_CUBE_BFUELG1_H */
