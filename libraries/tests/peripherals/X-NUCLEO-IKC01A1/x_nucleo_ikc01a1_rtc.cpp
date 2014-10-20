/**
******************************************************************************
* @file    x_nucleo_ikc01a1_rtc.cpp
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
    
/* Includes ------------------------------------------------------------------*/
#include "x_nucleo_ikc01a1.h"
#include "x_nucleo_ikc01a1_rtc.h"

/* Constants -----------------------------------------------------------------*/
/* Week and months names  */
static const char* weekDayNames[] = 
	{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
static const char* monthsNames[] = 
	{"January", "February", "March", "April", "May", "June", "July", 
	 "August", "September", "October", "November", "December"};

/* Methods -------------------------------------------------------------------*/
/** @brief Returns RTC Time of Day
  * @param tm: Pointer to a Time of Day descriptor structure
  * @retval 0 if ok, -1 if an I2C error has occured
  */
int M41T62::GetTime(rtc_time_t *tm) {
	uint8_t buf[M41T62_DATETIME_REG_SIZE];
	int ret;
	
	ret = expansion_board->io_read(buf, 
				       M41T62_SLAVE_ADDRESS,
				       M41T62_SSECONDS, 
				       M41T62_DATETIME_REG_SIZE);
	if(ret) return ret;

	tm->tm_sec = bcd2bin(buf[M41T62_SECONDS] & 0x7f);
	tm->tm_min = bcd2bin(buf[M41T62_MINUTES] & 0x7f);
	tm->tm_hour = bcd2bin(buf[M41T62_HOURS] & 0x3f);
	tm->tm_mday = bcd2bin(buf[M41T62_DATE] & 0x3f);
	tm->tm_wday = buf[M41T62_WDAY] & 0x07;
	tm->tm_mon = bcd2bin(buf[M41T62_MONTH] & 0x1f);
	tm->tm_year = bcd2bin(buf[M41T62_YEAR]) + 100 + 1900;

	return 0;
}

/** @brief Informs whether RTC Time of Day is valid or not.
  *        Used to figure out if RTC must be initialized or not
  * @retval 1 if Time of Day is valid, 0 if not valid, -1 if an I2C error has occured
  * @note: Oscillator Fail Bit must be cleared after at least four seconds,
  *        so application must call this function at least four seconds after RTC power on
  */
int M41T62::IsTimeOfDayValid(void)
{
  uint8_t val;
  int ret;

  /* Check whether Oscillator Failed bit is set  */
  ret = expansion_board->io_read(&val,
				 M41T62_SLAVE_ADDRESS,
				 M41T62_FLAGS, 1);
  if(ret) return ret;
  
  if(val & M41T62_FLAGS_OSCFAIL)
    return 0;
	
  return 1;
}

/** @brief Sets RTC time of day
  * @param tm: Pointer to a Time of Day descriptor structure
  * @retval 0 if ok, -1 if an I2C error has occured
  */
int M41T62::SetTime(rtc_time_t *tm)
{
  uint8_t buf[M41T62_DATETIME_REG_SIZE];
  int ret;

  ret = expansion_board->io_read(buf,
				 M41T62_SLAVE_ADDRESS,
				 M41T62_SSECONDS, 
				 M41T62_DATETIME_REG_SIZE);
  if(ret) return ret;

  /* Merge time-data and register flags into buf[0..7]  */
  buf[M41T62_SSECONDS] = 0;
  buf[M41T62_SECONDS] =
    bin2bcd(tm->tm_sec) | (buf[M41T62_SECONDS] & ~0x7f);
  buf[M41T62_MINUTES] =
    bin2bcd(tm->tm_min) | (buf[M41T62_MINUTES] & ~0x7f);
  buf[M41T62_HOURS] =
    bin2bcd(tm->tm_hour) | (buf[M41T62_HOURS] & ~0x3f) ;
  buf[M41T62_WDAY] =
    (tm->tm_wday & 0x07) | (buf[M41T62_WDAY] & ~0x07);
  buf[M41T62_DATE] =
    bin2bcd(tm->tm_mday) | (buf[M41T62_DATE] & ~0x3f);
  buf[M41T62_MONTH] =
    bin2bcd(tm->tm_mon) | (buf[M41T62_MONTH] & ~0x1f);
  /* assume 20YY not 19YY  */
  buf[M41T62_YEAR] = bin2bcd(tm->tm_year % 100);

  ret = expansion_board->io_write(buf,
				  M41T62_SLAVE_ADDRESS,
				  M41T62_SSECONDS, 
				  M41T62_DATETIME_REG_SIZE);
  if(ret) return ret;

  /* Clear Oscillator Failed bit  */
  ret = expansion_board->io_read(buf,
				 M41T62_SLAVE_ADDRESS,
				 M41T62_FLAGS, 1);
  if(ret) return ret;

  buf[0] &= ~M41T62_FLAGS_OSCFAIL;
  ret = expansion_board->io_write(buf,
				  M41T62_SLAVE_ADDRESS,
				  M41T62_FLAGS, 1);
  if(ret) return ret;

  return 0;
}

/** @brief Returns week day name
  * @param Week day number [1..7]
  * @retval Week name string if week day number is in [1..7] range. NULL otherwise
  */
const char* M41T62::GetWeekName(int week)
{
  if(week < 1 || week > 7)
    return NULL;
	
  return weekDayNames[week-1];
}

/** @brief Returns month day name
  * @param Month day number [1..12]
  * @retval Month name string if month day number is in [1..12] range. NULL otherwise
  */
const char* M41T62::GetMonthName(int month)
{
  if(month < 1 || month > 12)
    return NULL;
	
  return monthsNames[month-1];
}

