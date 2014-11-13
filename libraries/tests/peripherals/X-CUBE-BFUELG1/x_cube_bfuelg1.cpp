/**
******************************************************************************
* @file    x_cube_bfuelg1.cpp
* @author  AST / EST
* @version V0.0.1
* @date    08-October-2014
* @brief   Implementation file for the X_CUBE_BFUELG1 singleton class
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
#include "x_cube_bfuelg1.h"
#include "stc3115/stc3115.h"

/* Static variables ----------------------------------------------------------*/
X_CUBE_BFUELG1* X_CUBE_BFUELG1::_instance = NULL;

/* Methods -------------------------------------------------------------------*/
/**
 * @brief  Constructor
 * @param  bus reference to an DevI2C instance to be used for board communication
 */
X_CUBE_BFUELG1::X_CUBE_BFUELG1(void) : dev_i2c(BFUELG1_PIN_I2C_SDA, BFUELG1_PIN_I2C_SCL),
	charger(), 
	rtc(dev_i2c),
	gg(*(new STC3115(dev_i2c)))
{
}

/**
 * @brief  Get singleton instance
 * @param  bus reference to an DevI2C instance to be used for board communication
 * @return a pointer to the singleton instance of class X_CUBE_BFUELG1
 */
 X_CUBE_BFUELG1* X_CUBE_BFUELG1::Instance(void) {
	if(_instance == NULL) {
		_instance = new X_CUBE_BFUELG1();
	}
	return _instance;
}
