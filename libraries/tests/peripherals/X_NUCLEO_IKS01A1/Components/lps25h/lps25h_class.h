/**
 ******************************************************************************
 * @file    lps25h_class.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Header file for component LPS25H
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
 *       without specific prior written permission.
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

#ifndef __LPS25H_CLASS_H
#define __LPS25H_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "DevI2C.h"
#include "../Common/PressureSensor.h"
#include "../Common/TempSensor.h"
#include "lps25h.h"

/* Classes -------------------------------------------------------------------*/
/** Class representing a LPS25H sensor component
 */
class LPS25H : public PressureSensor, public TempSensor {
 public:
	/** Constructor
	 * @param i2c device I2C to be used for communication
	 */
        LPS25H(DevI2C &i2c) : PressureSensor(), TempSensor(), dev_i2c(i2c) {
		LPS25H_SlaveAddress = LPS25H_ADDRESS_HIGH;
	}
	
	/*** Interface Methods ***/
	virtual int Init(void *init_struct) {
		return LPS25H_Init((PRESSURE_InitTypeDef*)init_struct);
	}

	virtual int PowerOff(void) {
		return LPS25H_PowerOff();
	}

	virtual int ReadID(uint8_t *p_id) {
		return LPS25H_ReadID(p_id);
	}

	virtual int Reset(void) {
		return LPS25H_RebootCmd();
	}

	virtual void ConfigIT(uint16_t) { /* not yet implemented */ }
	virtual void EnableIT(uint8_t) { /* not yet implemented */ }
	virtual void DisableIT(uint8_t) { /* not yet implemented */ }
	virtual uint8_t ITStatus(uint16_t, uint16_t) { /* not yet implemented */ return 0; }
	virtual void ClearIT(uint16_t, uint16_t) { /* not yet implemented */ }
    
	virtual int GetPressure(float *pfData) {
		return LPS25H_GetPressure(pfData);
	}

	virtual int GetTemperature(float *pfData) {
		return LPS25H_GetTemperature(pfData);
	}

	virtual void AttachIT(void (*fptr)(void)) { /* not yet implemented */ }

	void SlaveAddrRemap(uint8_t SA0_Bit_Status) {
		LPS25H_SlaveAddrRemap(SA0_Bit_Status);
	}

protected:
	/*** Methods ***/
	PRESSURE_StatusTypeDef LPS25H_Init(PRESSURE_InitTypeDef *LPS25H_Init);
	PRESSURE_StatusTypeDef LPS25H_ReadID(uint8_t *p_id);
	PRESSURE_StatusTypeDef LPS25H_RebootCmd(void);
	PRESSURE_StatusTypeDef LPS25H_GetPressure(float* pfData);
	PRESSURE_StatusTypeDef LPS25H_GetTemperature(float* pfData);
	PRESSURE_StatusTypeDef LPS25H_PowerOff(void);
	void LPS25H_SlaveAddrRemap(uint8_t SA0_Bit_Status);
	
	PRESSURE_StatusTypeDef LPS25H_PowerOn(void);
	PRESSURE_StatusTypeDef LPS25H_I2C_ReadRawPressure(uint32_t *raw_press);
	PRESSURE_StatusTypeDef LPS25H_I2C_ReadRawTemperature(int16_t *raw_data);

	/**
	 * @brief  Configures LPS25H interrupt lines for NUCLEO boards
	 * @param  None
	 * @retval None
	 */
	void LPS25H_IO_ITConfig(void)
	{
		/* To be implemented */
	}

	/**
	 * @brief  Configures LPS25H I2C interface
	 * @param  None
	 * @retval PRESSURE_OK in case of success, an error code otherwise
	 */
	PRESSURE_StatusTypeDef LPS25H_IO_Init(void)
	{
		return PRESSURE_OK; /* done in constructor */
	}

	/**
	 * @brief utility function to read data from STC3115
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval PRESSURE_OK if ok, PRESSURE_ERROR if an I2C error has occured
	 */
	PRESSURE_StatusTypeDef LPS25H_IO_Read(uint8_t* pBuffer, 
					      uint8_t RegisterAddr, uint16_t NumByteToRead)
	{
		int ret = dev_i2c.i2c_read(pBuffer,
					   LPS25H_SlaveAddress,
					   RegisterAddr,
					   NumByteToRead);
		if(ret != 0) {
			return PRESSURE_ERROR;
		}
		return PRESSURE_OK;
	}
	
	/**
	 * @brief utility function to write data to STC3115
	 * @param  pBuffer: pointer to buffer to be filled.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToWrite: number of bytes to write.
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	PRESSURE_StatusTypeDef LPS25H_IO_Write(uint8_t* pBuffer, 
					       uint8_t RegisterAddr, uint16_t NumByteToWrite)
	{
		int ret = dev_i2c.i2c_write(pBuffer,
					    LPS25H_SlaveAddress,
					    RegisterAddr,
					    NumByteToWrite);
		if(ret != 0) {
			return PRESSURE_ERROR;
		}
		return PRESSURE_OK;
	}
	
	/*** Instance Variables ***/
	/* IO Device */
	DevI2C &dev_i2c;

	uint8_t LPS25H_SlaveAddress;
};

#endif // __LPS25H_CLASS_H
