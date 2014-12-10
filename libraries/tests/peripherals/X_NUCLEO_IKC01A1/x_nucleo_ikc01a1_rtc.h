/**
******************************************************************************
* @file    x_nucleo_ikc01a1_rtc.h
* @author  AST / EST
* @version V0.0.1
* @date    08-October-2014
* @brief   Header file for RTC component M41T62
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
#include "x_nucleo_ikc01a1_crtc.h"
#include "x_nucleo_ikc01a1_targets.h"
#include "x_nucleo_ikc01a1_i2c.h"

/* Macros -------------------------------------------------------------------*/
/* M41T62 */
#define M41T62_SLAVE_ADDRESS        0xD0    /* M41T62 8-bit address byte (0xD0 8 bit)		*/

#define M41T62_SSECONDS             0x00
#define M41T62_SECONDS              0x01
#define M41T62_MINUTES              0x02
#define M41T62_HOURS                0x03
#define M41T62_WDAY                 0x04
#define M41T62_DATE                 0x05
#define M41T62_MONTH                0x06
#define M41T62_YEAR                 0x07
#define M41T62_CALIBRATION          0x08
#define M41T62_WATCHDOG             0x09
#define M41T62_ALARM_MONTH          0x0A
#define M41T62_ALARM_DAY            0x0B
#define M41T62_ALARM_HOUR           0x0C
#define M41T62_ALARM_MINUTES        0x0D
#define M41T62_ALARM_SECONDS        0x0E
#define M41T62_FLAGS                0x0F

#define M41T62_DATETIME_REG_SIZE    (M41T62_YEAR + 1)
#define M41T62_ALARM_REG_SIZE       (M41T62_FLAGS - M41T62_ALARM_MONTH)

#define M41T62_SEC_ST		    (1 << 7)	/* ST: Stop Bit */
#define M41T62_ALMON_AFE	    (1 << 7)	/* AFE: AF Enable Bit */
#define M41T62_ALMON_SQWE	    (1 << 6)	/* SQWE: SQW Enable Bit */
#define M41T62_ALHOUR_HT	    (1 << 6)	/* HT: Halt Update Bit */
#define M41T62_FLAGS_ALARMF	    (1 << 6)	/* AF: Alarm Flag Bit */
#define M41T62_FLAGS_OSCFAIL        (1 << 2)    /* Oscillator fail bit */
#define M41T62_FLAGS_STOP           (1 << 7)    /* Stop Bit. Used to restart oscillator */

/* Classes -------------------------------------------------------------------*/
/** Class representing a M41T62 real-time clock
 */
class M41T62
{
public:
    /** Constructor
     * @param i2c device I2C used to communicate on battery expansion board
     */
    M41T62(DevI2C &i2c) : irq_out(RTC_PIN_IRQ_OUT),
        dev_i2c(i2c) {
        ClearAlarm();
        ClearIrq();
    };

    int GetTime(rtc_time_t*);
    int SetTime(rtc_time_t*);
    int IsTimeOfDayValid(void);
    int RestartOscillator(void);
    int SetAlarm(rtc_alarm_t*);
    int ClearAlarm(void);
    int ClearIrq(void);

    /** Attach a function to call when a falling edge occurs on the input
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     */
    void attach_irq(void (*fptr)(void)) {
        irq_out.fall(fptr);
    }

    /** Enable IRQ. This method depends on hw implementation, might enable one
     *  port interrupts. For further information, check gpio_irq_enable().
     */
    void enable_irq() {
        irq_out.enable_irq();
    }

    /** Disable IRQ. This method depends on hw implementation, might disable one
     *  port interrupts. For further information, check gpio_irq_disable().
     */
    void disable_irq() {
        irq_out.disable_irq();
    }

    static const char* GetWeekName(int);
    static const char* GetMonthName(int);

protected:
    /* BCD helper functions  */
    static unsigned int bcd2bin(uint8_t val) {
        return ((val) & 0x0f) + ((val) >> 4)  * 10;
    }

    static uint8_t bin2bcd (unsigned int val) {
        return (((val / 10) << 4) | (val % 10));
    }

    /* alarm helper function */
    int prepare_alarm_buffer(uint8_t *buf, rtc_alarm_t *alm);

private:
    InterruptIn irq_out;
    DevI2C &dev_i2c;
};

#endif // __X_NUCLEO_IKC01A1_RTC_H
