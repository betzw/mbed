/**
 ******************************************************************************
 * @file    hts221_class.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Header file for component HTS221
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

#ifndef __HTS221_CLASS_H
#define __HTS221_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "DevI2C.h"
#include "../Interfaces/HumiditySensor.h"
#include "../Interfaces/TempSensor.h"
#include "hts221.h"

/* Classes -------------------------------------------------------------------*/
/** Class representing a HTS221 sensor component
 */
class HTS221 : public HumiditySensor, public TempSensor {
 public:
	/** Constructor
	 * @param i2c device I2C to be used for communication
	 */
        HTS221(DevI2C &i2c) : HumiditySensor(), TempSensor(), dev_i2c(i2c) {
		T0_degC = T1_degC = H0_rh = H1_rh = 0.0;
		T0_out = T1_out = H0_T0_out = H1_T0_out = 0;
	}
	
	/** Destructor
	 */
        virtual ~HTS221() {}
	
	/*** Interface Methods ***/
	virtual int Init(void *init_struct) {
		return HTS221_Init((HUM_TEMP_InitTypeDef*)init_struct);
	}

	/**
	 * @brief       Enter sensor shutdown mode
	 * @return      0 in case of success, an error code otherwise
	 */
	virtual int PowerOff(void) {
		return HTS221_Power_OFF();
	}

	virtual int ReadID(uint8_t *ht_id) {
		return HTS221_ReadID(ht_id);
	}

	/**
	 * @brief       Reset sensor
	 * @return      0 in case of success, an error code otherwise
	 */
	virtual int Reset(void) {
		return HTS221_RebootCmd();
	}

	virtual int GetHumidity(float *pfData) {
		return HTS221_GetHumidity(pfData);
	}

	virtual int GetTemperature(float *pfData) {
		return HTS221_GetTemperature(pfData);
	}

 protected:
	/*** Methods ***/
	HUM_TEMP_StatusTypeDef HTS221_Init(HUM_TEMP_InitTypeDef *HTS221_Init);
	HUM_TEMP_StatusTypeDef HTS221_Power_OFF(void);
	HUM_TEMP_StatusTypeDef HTS221_ReadID(uint8_t *ht_id);
	HUM_TEMP_StatusTypeDef HTS221_RebootCmd(void);
	HUM_TEMP_StatusTypeDef HTS221_GetHumidity(float* pfData);
	HUM_TEMP_StatusTypeDef HTS221_GetTemperature(float* pfData);

	HUM_TEMP_StatusTypeDef HTS221_Power_On(void);
	HUM_TEMP_StatusTypeDef HTS221_Calibration(void);

	/**
	 * @brief  Configures HTS221 interrupt lines for NUCLEO boards
	 * @param  None
	 * @retval None
	 */
	void HTS221_IO_ITConfig(void)
	{
		/* To be implemented */
	}

	/**
	 * @brief  Configures HTS221 I2C interface
	 * @param  None
	 * @retval HUM_TEMP_OK in case of success, an error code otherwise
	 */
	HUM_TEMP_StatusTypeDef HTS221_IO_Init(void)
	{
		return HUM_TEMP_OK; /* done in constructor */
	}

	/**
	 * @brief utility function to read data from STC3115
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval HUM_TEMP_OK if ok, HUM_TEMP_ERROR if an I2C error has occured
	 */
	HUM_TEMP_StatusTypeDef HTS221_IO_Read(uint8_t* pBuffer, 
					      uint8_t RegisterAddr, uint16_t NumByteToRead)
	{
		int ret = dev_i2c.i2c_read(pBuffer,
					   HTS221_ADDRESS,
					   RegisterAddr,
					   NumByteToRead);
		if(ret != 0) {
			return HUM_TEMP_ERROR;
		}
		return HUM_TEMP_OK;
	}
	
	/**
	 * @brief utility function to write data to STC3115
	 * @param  pBuffer: pointer to buffer to be filled.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToWrite: number of bytes to write.
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	HUM_TEMP_StatusTypeDef HTS221_IO_Write(uint8_t* pBuffer, 
					       uint8_t RegisterAddr, uint16_t NumByteToWrite)
	{
		int ret = dev_i2c.i2c_write(pBuffer,
					    HTS221_ADDRESS,
					    RegisterAddr,
					    NumByteToWrite);
		if(ret != 0) {
			return HUM_TEMP_ERROR;
		}
		return HUM_TEMP_OK;
	}
	
	/*** Instance Variables ***/
	/* IO Device */
	DevI2C &dev_i2c;

	/* Temperature in degree for calibration  */
	float T0_degC, T1_degC;
	
	/* Output temperature value for calibration */
	int16_t T0_out, T1_out;
	
	/* Humidity for calibration  */
	float H0_rh, H1_rh;
	
	/* Output Humidity value for calibration */
	int16_t H0_T0_out, H1_T0_out;
};

#endif // __HTS221_CLASS_H
