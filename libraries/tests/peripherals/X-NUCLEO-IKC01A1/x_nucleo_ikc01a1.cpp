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

/* Static variables ----------------------------------------------------------*/
X_NUCLEO_IKC01A1* X_NUCLEO_IKC01A1::_instance = NULL;

/* Methods -------------------------------------------------------------------*/
X_NUCLEO_IKC01A1* X_NUCLEO_IKC01A1::Instance() {
	if(_instance == NULL) {
		_instance = new X_NUCLEO_IKC01A1();
	}
	return _instance;
}

/**
 * @brief  Writes a buffer from the I2C peripheral device.
 * @param  pBuffer: pointer to data to be read.
 * @param  DaviceAddr: specifies the peripheral device slave address.
 * @param  RegisterAddr: specifies internal address register to read from.
 * @param  NumByteToRead: number of bytes to be read.
 * @retval 0 if ok, -1 if an I2C error has occured
 */
int X_NUCLEO_IKC01A1::io_write(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, 
			       uint16_t NumByteToRead)
{
	int ret;
	uint8_t tmp[16];
	
	/* First, send device address. Then, send data and STOP condition */
	tmp[0] = RegisterAddr;
	memcpy(tmp+1, pBuffer, NumByteToRead);

	ret = i2c.write(DeviceAddr, (const char*)tmp, NumByteToRead+1, 0);

	if(ret) {
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
int X_NUCLEO_IKC01A1::io_read(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, 
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
		return -1;
	}
	return 0;
}

