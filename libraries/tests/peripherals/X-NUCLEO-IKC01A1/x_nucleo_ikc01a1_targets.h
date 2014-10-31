/**
  ******************************************************************************
  * @file    x_nucleo_ikc01a1_targets.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    08-October-2014
  * @brief   This header file is intended to manage the differences between 
  *          the different supported base-boards which might mount the
  *          X_NUCLEO_IKC01A1 battery management expansion board.
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
#ifndef _X_NUCLEO_IKC01A1_TARGETS_H_
#define _X_NUCLEO_IKC01A1_TARGETS_H_

#if defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L053R8)
/* Includes -------------------------------------------------------------------*/
#include "stc3115/stc3115_Driver.h"
#include "stc3115/stc3115_Battery.h"

/*** Charger ***/
#define CHARGER_PIN_ST1             (PC_2)
#define CHARGER_PIN_ST2             (PC_3) 
#define CHARGER_PIN_DISCHARGE       (PB_0)


/*** I2C ***/
#define IKC01A1_PIN_I2C_SCL         (PB_8)
#define IKC01A1_PIN_I2C_SDA         (PB_9)


/*** RTC ***/
#define RTC_PIN_IRQ_OUT             (PA_5)

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


/*** Gas Gauge ***/
#define GG_PIN_ALM                  (PC_7)


#else // !(defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L053R8))

#error "Platform not supported!"

#endif // !(defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L053R8))


#endif // _X_NUCLEO_IKC01A1_TARGETS_H_
