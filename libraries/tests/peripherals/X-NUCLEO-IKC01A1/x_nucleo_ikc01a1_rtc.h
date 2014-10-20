/**
  ******************************************************************************
  * @file    x_nucleo_ikc01a1_rtc.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    
  * @brief   
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __X_NUCLEO_IKC01A1_RTC_H
#define __X_NUCLEO_IKC01A1_RTC_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1_targets.h"

/* Forward declarations ------------------------------------------------------*/
class X_NUCLEO_IKC01A1;

/* Types ---------------------------------------------------------------------*/
/** Structure defining Time of Day
 */
typedef struct {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
} rtc_time_t ;
   
/** Structure defining an alarm
 */
typedef enum {
	ONCE_PER_SEC,
	ONCE_PER_MIN,
	ONCE_PER_HOUR,
	ONCE_PER_DAY,
	ONCE_PER_MONTH,
	ONCE_PER_YEAR
} rtc_repeat_t;

typedef struct {
	int alm_sec;
	int alm_min;
	int alm_hour;
	int alm_day;
	int alm_mon;
	rtc_repeat_t alm_repeat_mode;
} rtc_alarm_t ;

/* Classes -------------------------------------------------------------------*/
class M41T62 {
 public:
        M41T62(X_NUCLEO_IKC01A1 *board) : irq_out(RTC_PIN_IRQ_OUT), 
		expansion_board(board) {};

	int GetTime(rtc_time_t*);
	int SetTime(rtc_time_t*);
	int IsTimeOfDayValid(void);
	void RestartOscillator(void);
	int SetAlarm(rtc_alarm_t*);
	int ClearAlarm(void);
	int ClearIrq(void);

	static const char* GetWeekName(int);
	static const char* GetMonthName(int);

 protected:
	/* BCD helper functions  */
	static inline unsigned int bcd2bin(uint8_t val)
	{
		return ((val) & 0x0f) + ((val) >> 4)  * 10;
	}
	
	static inline uint8_t bin2bcd (unsigned int val)
	{
		return (((val / 10) << 4) | (val % 10));
	}

 private:
	InterruptIn irq_out;
	X_NUCLEO_IKC01A1 *expansion_board;
};

#endif // __X_NUCLEO_IKC01A1_RTC_H
