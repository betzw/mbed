/**
  ******************************************************************************
  * @file    x_nucleo_ikc01a1_charger.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    08-October-2014
  * @brief   This file contains the common defines and functions prototypes for
  *          the x_nucleo_ikc01a1_charger.cpp driver.
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
#ifndef __X_CUBE_BFUELG1_CHARGER_H
#define __X_CUBE_BFUELG1_CHARGER_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1_targets.h"

/* Typedefs ------------------------------------------------------------------*/
typedef enum charger_conditions {
	CHARGER_CHARGE_IN_PROGRESS,
	CHARGER_CHARGE_DONE,
	CHARGER_IN_STAND_BY,
	CHARGER_INVALID_STATE
} charger_conditions_t;

/* Classes  ------------------------------------------------------------------*/
class L6924D {
 public:
        L6924D(void) :  discharge(CHARGER_PIN_DISCHARGE), st1(CHARGER_PIN_ST1), st2(CHARGER_PIN_ST2) {
		discharge = 0;	
	}

	charger_conditions_t GetState(void) {
		if(!st1 && st2)
			return CHARGER_CHARGE_IN_PROGRESS;
		
		if(st1 && !st2)
			return CHARGER_CHARGE_DONE;
		
		if(st1 && st2)
			return CHARGER_IN_STAND_BY;
		
		return CHARGER_INVALID_STATE;
	}

	DigitalOut discharge;

 private:
	DigitalIn st1;
	DigitalIn st2;
};

#endif /* __X_CUBE_BFUELG1_CHARGER_H */
