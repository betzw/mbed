/**
  ******************************************************************************
  * @file    x_nucleo_ikc01a1.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    08-October-2014
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

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __X_NUCLEO_IKC01A1_H
#define __X_NUCLEO_IKC01A1_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1_targets.h"
#include "x_nucleo_ikc01a1_charger.h"
#include "x_nucleo_ikc01a1_rtc.h"
#include "x_nucleo_ikc01a1_gg.h"

/* Classes -------------------------------------------------------------------*/
/* Class X_NUCLEO_IKC01A1 is intended to represent the battery management 
   expansion board with the same name, featuring basically three IPs:
   1) a battery charger of class L6924D
   2) a real-time clock (RTC) of class M41T62
   3) and a gas gauge (GG) of class STC3115
   It is intentionally implemented as a singleton because only one 
   X_NUCLEO_IKC01A1 at a time might be deployed in a HW component stack.
   In order to get the singleton instance you have to call class method
   X_NUCLEO_IKC01A1::Instance().
*/
class X_NUCLEO_IKC01A1 {
 protected:
        X_NUCLEO_IKC01A1() : i2c(IKC01A1_PIN_I2C_SDA, IKC01A1_PIN_I2C_SCL), 
		charger(), 
		rtc(this),
		gg(this)
		{};

	I2C i2c;

 public:
	static X_NUCLEO_IKC01A1* Instance();

	int io_read(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint16_t NumByteToRead);
	int io_write(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, uint16_t NumByteToRead);

	L6924D charger;
	M41T62 rtc;
	STC3115 gg;

 private:
	static X_NUCLEO_IKC01A1 *_instance;
};

#endif /* __X_NUCLEO_IKC01A1_H */
