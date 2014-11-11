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

/** @addtogroup X_CUBE_BFUELG1_Demonstration
  * @{
  */ 

/** @defgroup X_CUBE_BFUELG1_Demonstration_CharacterUserInterface
 * @{
 */
 
/** @brief Fills a rtc_time structure according to user input
 * @param time Pointer to the structure to fill
 * @retval none
 */
void cuiGetTime(rtc_time_t* time)
{
	int ret;
	
	printf("Please set RTC clock\r\n");
	do{
		printf("Enter Year: ");
		ret = scanf("%u", &time->tm_year);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
	
	do{
		printf("Enter Month [1..12]: ");
		ret = scanf("%u", &time->tm_mon);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
	
	do{
		printf("Enter Month day [1..31]: ");
		ret = scanf("%u", &time->tm_mday);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
	
	do{
		printf("Enter Week Day [1..7]: ");
		ret = scanf("%u", &time->tm_wday);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
		
	do{
		printf("Enter Hours [0..23]: ");
		ret = scanf("%u", &time->tm_hour);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
	
	do{
		printf("Enter Minutes [0..59]: ");
		ret = scanf("%u", &time->tm_min);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
	
	do{
		printf("Enter Seconds [0..59]: ");
		ret = scanf("%u", &time->tm_sec);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);	
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
	int ret;
	
	do{
		printf("Enter SOC Alarm Threshold [%%]: ");
		ret = scanf("%i", socThreshold);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
	
	do{
		printf("Enter Voltage Alarm Threshold [mV]: ");
		ret = scanf("%i", voltageThreshold);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);

	do{
		printf("Enter periodic RTC alarm [sec]: ");
		ret = scanf("%i", periodicRtcAlarm);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);

	do{
		printf("Enable discharging [0|1]: ");
		ret = scanf("%i", enableDischarging);
		if(ret != 1)
			printf("\r\n");
	}while(ret != 1);
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
