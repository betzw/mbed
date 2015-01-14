/**
 ******************************************************************************
 * @file    userinterface.c
 * @author  AST
 * @version V0.0.1
 * @date    15-Oct-2014
 * @brief   This file implements application Command User Interface
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

#include <stdio.h>
#include "userinterface.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup X_NUCLEO_IKC01A1_Demonstration
  * @{
  */ 

/** @defgroup X_NUCLEO_IKC01A1_Demonstration_CharacterUserInterface
 * @{
 */

/** @brief Asks user to input an integer number and returns its value
 * @param message A message to be prompted on the console when asking value
 * @retval The integer acquired from console
 */
static int cuiGetInteger(const char* message)
{
	int ret, value;
	char buf[32];
		
	do{
		printf(message);
		fflush(stdout);
		
		ret = scanf("%u", &value);
		if(ret != 1){
		  	/* In case of non-matching characters, eat stdin as IAR and
		     * TrueStudio won't flush it as Keil does */
		  	scanf("%s", buf);
			printf("\r\nPlease insert a valid integer number\r\n");
		}
	}while(ret != 1);
	
	return value;
}


/** @brief Fills a rtc_time structure according to user input
 * @param time Pointer to the structure to fill
 * @retval none
 */
void cuiGetTime(rtc_time_t* time)
{
	printf("Please set RTC clock\r\n");
	
	time->tm_year = cuiGetInteger("Enter Year: ");
	time->tm_mon = cuiGetInteger("Enter Month [1..12]: ");
	time->tm_mday = cuiGetInteger("Enter Month day [1..31]: ");
	time->tm_wday = cuiGetInteger("Enter Week Day [1..7]: ");
	time->tm_hour = cuiGetInteger("Enter Hours [0..23]: ");
	time->tm_min = cuiGetInteger("Enter Minutes [0..59]: ");
	time->tm_sec = cuiGetInteger("Enter Seconds [0..59]: ");
}


/** @brief Asks user SoC, Voltage, RTC periodic alarm thresholds, and discharging info
 * @param socThreshold Pointer to State of Charge alarm threshold variable
 * @param voltageThreshold Pointer to Voltage alarm threshold variable
 * @param periodicRtcAlarm Pointer to periodic RTC alarm variable
 * @param enableDischarging Pointer to discharging state variable
 * @retval none
 */
void getUserInputParams(int* socThreshold, int* voltageThreshold, int* periodicRtcAlarm, int* enableDischarging)
{
	*socThreshold = cuiGetInteger("Enter SOC Alarm Threshold [%%]: ");
	*voltageThreshold = cuiGetInteger("Enter Voltage Alarm Threshold [mV]: ");
	*periodicRtcAlarm = cuiGetInteger("Enter periodic RTC alarm [sec]: ");
	*enableDischarging = cuiGetInteger("Enable discharging [0|1]: ");
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
