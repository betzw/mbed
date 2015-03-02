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

#if defined(TARGET_STM) // STM Nucleo Boards (use ST Morpho headers)
/*** Charger ***/
#define CHARGER_PIN_ST1             (PC_2) /* NOTE: this pin exists only on the Morpho connector and requires a HW patch for Arduino-based boards */
#define CHARGER_PIN_ST2             (PC_3) /* NOTE: this pin exists only on the Morpho connector and requires a HW patch for Arduino-based boards */
#define CHARGER_PIN_DISCHARGE       (PB_0)


/*** I2C ***/
#define IKC01A1_PIN_I2C_SCL         (PB_8)
#define IKC01A1_PIN_I2C_SDA         (PB_9)


/*** RTC ***/
#define RTC_PIN_IRQ_OUT             (PA_5)


/*** Gas Gauge ***/
#define GG_PIN_ALM                  (PC_7)

#elif defined(TARGET_K64F) || defined(TARGET_LPC11U68) // Freescale & NXP (use Arduino headers)

/*** Charger ***/
#if defined(TARGET_K64F)
#define CHARGER_PIN_ST1             (A5) /* moved to closest Arduino connector pin (NOTE: there is currently a bug in file 'PinNames.h' of the mbed porting for this platform) */
#define CHARGER_PIN_ST2             (A4) /* moved to closest Arduino connector pin (NOTE: there is currently a bug in file 'PinNames.h' of the mbed porting for this platform) */
#elif defined(TARGET_LPC11U68)
#define CHARGER_PIN_ST1             (D7) /* A4 corresponds to P0_5 which conflicts with I2C SDA on this board, therefore need to choose another free (digital) pin */
#define CHARGER_PIN_ST2             (D6) /* A5 corresponds to P0_4 which conflicts with I2C SCL on this board, therefore need to choose another free (digital) pin */
#endif // defined(TARGET_K64F)

#define CHARGER_PIN_DISCHARGE       (A3) /* corresponds to Morpho connector */

/*** I2C ***/
#define IKC01A1_PIN_I2C_SCL         (D15) /* corresponds to Morpho connector */
#define IKC01A1_PIN_I2C_SDA         (D14) /* corresponds to Morpho connector */


/*** RTC ***/
#define RTC_PIN_IRQ_OUT             (D13) /* corresponds to Morpho connector */


/*** Gas Gauge ***/
#if defined(TARGET_K64F)
#define GG_PIN_ALM                  (D9) /* corresponds to Morpho connector */
#elif defined(TARGET_LPC11U68)
#define GG_PIN_ALM                  (D8) /* D9 - being on port #2 - is badly ported to mbed on this platform, therefore choosing a more stable pin with D8 (NOTE: requires a HW patch) */
#endif // defined(TARGET_K64F)

#else // !(defined(TARGET_K64F) || defined(TARGET_LPC11U68))
#error "Platform not supported!"
#endif // !defined(TARGET_STM)

#endif // _X_NUCLEO_IKC01A1_TARGETS_H_
