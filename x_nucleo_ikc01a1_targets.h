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

#if defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L053R8) // STM Nucleo Boards (use ST Morpho headers)
/*** Charger ***/
#define CHARGER_PIN_ST1             (PC_2)
#define CHARGER_PIN_ST2             (PC_3) 
#define CHARGER_PIN_DISCHARGE       (PB_0)


/*** I2C ***/
#define IKC01A1_PIN_I2C_SCL         (PB_8)
#define IKC01A1_PIN_I2C_SDA         (PB_9)


/*** RTC ***/
#define RTC_PIN_IRQ_OUT             (PA_5)


/*** Gas Gauge ***/
#define GG_PIN_ALM                  (PC_7)

#elif defined(TARGET_K64F) // Freescale (use Arduino headers)

/*** Charger ***/
#define CHARGER_PIN_ST1             (A4)
#define CHARGER_PIN_ST2             (A5)
#define CHARGER_PIN_DISCHARGE       (A3)


/*** I2C ***/
#define IKC01A1_PIN_I2C_SCL         (D15)
#define IKC01A1_PIN_I2C_SDA         (D14)


/*** RTC ***/
#define RTC_PIN_IRQ_OUT             (D13)


/*** Gas Gauge ***/
#define GG_PIN_ALM                  (D9)

#else // !(defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L053R8) || defined(TARGET_MCU_K64F))
#error "Platform not supported!"
#endif // !(defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L053R8))

#endif // _X_NUCLEO_IKC01A1_TARGETS_H_
