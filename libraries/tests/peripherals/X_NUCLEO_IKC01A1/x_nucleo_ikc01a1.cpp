/**
******************************************************************************
* @file    x_nucleo_ikc01a1.cpp
* @author  AST / EST
* @version V0.0.1
* @date    08-October-2014
* @brief   Implementation file for the X_NUCLEO_IKC01A1 singleton class
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
#include "mbed.h"
#include "x_nucleo_ikc01a1.h"
#include "stc3115/stc3115.h"

/* Static variables ----------------------------------------------------------*/
X_NUCLEO_IKC01A1* X_NUCLEO_IKC01A1::_instance = NULL;

/* Methods -------------------------------------------------------------------*/
/**
 * @brief  Constructor
 */
X_NUCLEO_IKC01A1::X_NUCLEO_IKC01A1(void) : dev_i2c(IKC01A1_PIN_I2C_SDA, IKC01A1_PIN_I2C_SCL),
	charger(), 
	rtc(dev_i2c),
	gg(*(new STC3115(dev_i2c)))
{
}

/**
 * @brief  Get singleton instance
 * @return a pointer to the singleton instance of class X_NUCLEO_IKC01A1
 */
 X_NUCLEO_IKC01A1* X_NUCLEO_IKC01A1::Instance(void) {
	if(_instance == NULL) {
		wait(1); /* betzw - To BE REVIEWED: 
			    wait a second to give GG time to startup */

		_instance = new X_NUCLEO_IKC01A1();
	}
	return _instance;
}
