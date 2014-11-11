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
X_CUBE_BFUELG1::X_CUBE_BFUELG1(I2C &bus) : i2c(bus), 
	charger(), 
	rtc(this),
	gg(*(new STC3115(this))) 
{
}

X_CUBE_BFUELG1* X_CUBE_BFUELG1::Instance(I2C &bus) {
	if(_instance == NULL) {
		_instance = new X_CUBE_BFUELG1(bus);
	}
	return _instance;
}

/**
 * @brief  Writes a buffer from the I2C peripheral device.
 * @param  pBuffer: pointer to data to be read.
 * @param  DeviceAddr: specifies the peripheral device slave address.
 * @param  RegisterAddr: specifies internal address register to read from.
 * @param  NumByteToWrite: number of bytes to be written.
 * @retval 0 if ok, -1 if an I2C error has occured
 */
int X_CUBE_BFUELG1::io_write(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, 
			       uint16_t NumByteToWrite)
{
	int ret;
	uint8_t tmp[32];
	
	/* First, send device address. Then, send data and STOP condition */
	tmp[0] = RegisterAddr;
	memcpy(tmp+1, pBuffer, NumByteToWrite);

	ret = i2c.write(DeviceAddr, (const char*)tmp, NumByteToWrite+1, 0);

	if(ret) {
		error("%s: dev = %d, reg = %d, num = %d\n",
		      __func__, DeviceAddr, RegisterAddr, NumByteToWrite);
		return -1;
	}
	return 0;
}

/**
 * @brief  Reads a buffer from the I2C peripheral device.
 * @param  pBuffer: pointer to data to be read.
 * @param  DaviceAddr: specifies the peripheral device slave address.
 * @param  RegisterAddr: specifies internal address register to read from.
 * @param  NumByteToRead: number of bytes to be read.
 * @retval 0 if ok, -1 if an I2C error has occured
 */
int X_CUBE_BFUELG1::io_read(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, 
			      uint16_t NumByteToRead)
{
	int ret;

	/* Send device address, with no STOP condition */
	ret = i2c.write(DeviceAddr, (const char*)&RegisterAddr, 1, 1);
	if(!ret) {
		/* Read data, with STOP condition  */
		ret = i2c.read(DeviceAddr, (char*)pBuffer, NumByteToRead, 0);
	}

	if(ret) {
		error("%s: dev = %d, reg = %d, num = %d\n",
		      __func__, DeviceAddr, RegisterAddr, NumByteToRead);
		return -1;
	}
	return 0;
}

