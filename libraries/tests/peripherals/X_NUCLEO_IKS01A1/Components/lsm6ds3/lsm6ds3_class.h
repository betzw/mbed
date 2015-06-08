/**
 ******************************************************************************
 * @file    lsm6ds3_class.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Header file for component LSM6DS3
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

#ifndef __LSM6DS3_CLASS_H
#define __LSM6DS3_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "DevI2C.h"
#include "../Interfaces/GyroSensor.h"
#include "../Interfaces/MotionSensor.h"
#include "lsm6ds3.h"

/* Classes -------------------------------------------------------------------*/
/** Class representing a LSM6DS3 sensor component
 */
class LSM6DS3 : public GyroSensor, public MotionSensor {
 public:
	/** Constructor
	 * @param i2c device I2C to be used for communication
	 */
        LSM6DS3(DevI2C &i2c) : GyroSensor(), MotionSensor(), dev_i2c(i2c) {
	}
	
	/** Destructor
	 */
        virtual ~LSM6DS3() {}
	
	/*** Interface Methods ***/
	virtual int Init(void *init_struct) {
		return LSM6DS3_Init((IMU_6AXES_InitTypeDef*)init_struct);
	}

	virtual int ReadID(uint8_t *xg_id) {
		return LSM6DS3_Read_XG_ID(xg_id);
	}

	virtual int Get_X_Axes(int32_t *pData) {
		return LSM6DS3_X_GetAxes(pData);
	}

	virtual int Get_X_AxesRaw(int16_t *pData) {
		return LSM6DS3_X_GetAxesRaw(pData);
	}

	virtual int Get_G_Axes(int32_t *pData) {
		return LSM6DS3_G_GetAxes(pData);
	}

	virtual int Get_G_AxesRaw(int16_t *pData) {
		return LSM6DS3_G_GetAxesRaw(pData);
	}

	virtual int Get_X_ODR(float *odr) {
		return LSM6DS3_X_Get_ODR(odr);
	}

	virtual int Set_X_ODR(float odr) {
		return LSM6DS3_X_Set_ODR(odr);
	}

	virtual int Get_X_Sensitivity(float *pfData) {
		return LSM6DS3_X_GetSensitivity(pfData);
	}

	virtual int Get_X_FS(float *fullScale) {
		return LSM6DS3_X_Get_FS(fullScale);
	}

	virtual int Set_X_FS(float fullScale) {
		return LSM6DS3_X_Set_FS(fullScale);
	}

	virtual int Get_G_ODR(float *odr) {
		return LSM6DS3_G_Get_ODR(odr);
	}

	virtual int Set_G_ODR(float odr) {
		return LSM6DS3_G_Set_ODR(odr);
	}

	virtual int Get_G_Sensitivity(float *pfData) {
		return LSM6DS3_G_GetSensitivity(pfData);
	}

	virtual int Get_G_FS(float *fullScale) {
		return LSM6DS3_G_Get_FS(fullScale);
	}

	virtual int Set_G_FS(float fullScale) {
		return LSM6DS3_G_Set_FS(fullScale);
	}

	/* Additional Public Methods */
	IMU_6AXES_StatusTypeDef Enable_Free_Fall_Detection(void) {
		return LSM6DS3_Enable_Free_Fall_Detection();
	}

	IMU_6AXES_StatusTypeDef Disable_Free_Fall_Detection(void) {
		return LSM6DS3_Disable_Free_Fall_Detection();
	}

	IMU_6AXES_StatusTypeDef Get_Status_Free_Fall_Detection(uint8_t *status) {
		return LSM6DS3_Get_Status_Free_Fall_Detection(status);
	}

 protected:
	/*** Methods ***/
	IMU_6AXES_StatusTypeDef LSM6DS3_Init(IMU_6AXES_InitTypeDef *LSM6DS3_Init);
	IMU_6AXES_StatusTypeDef LSM6DS3_Read_XG_ID(uint8_t *xg_id);
	IMU_6AXES_StatusTypeDef LSM6DS3_X_GetAxes(int32_t *pData);
	IMU_6AXES_StatusTypeDef LSM6DS3_X_GetAxesRaw(int16_t *pData);
	IMU_6AXES_StatusTypeDef LSM6DS3_G_GetAxes(int32_t *pData);
	IMU_6AXES_StatusTypeDef LSM6DS3_G_GetAxesRaw(int16_t *pData);
	IMU_6AXES_StatusTypeDef LSM6DS3_X_Get_ODR( float *odr );
	IMU_6AXES_StatusTypeDef LSM6DS3_X_Set_ODR( float odr );
	IMU_6AXES_StatusTypeDef LSM6DS3_X_GetSensitivity( float *pfData );
	IMU_6AXES_StatusTypeDef LSM6DS3_X_Get_FS( float *fullScale );
	IMU_6AXES_StatusTypeDef LSM6DS3_X_Set_FS( float fullScale );
	IMU_6AXES_StatusTypeDef LSM6DS3_G_Get_ODR( float *odr );
	IMU_6AXES_StatusTypeDef LSM6DS3_G_Set_ODR( float odr );
	IMU_6AXES_StatusTypeDef LSM6DS3_G_GetSensitivity( float *pfData );
	IMU_6AXES_StatusTypeDef LSM6DS3_G_Get_FS( float *fullScale );
	IMU_6AXES_StatusTypeDef LSM6DS3_G_Set_FS( float fullScale );
	IMU_6AXES_StatusTypeDef LSM6DS3_Enable_Free_Fall_Detection( void );
	IMU_6AXES_StatusTypeDef LSM6DS3_Disable_Free_Fall_Detection( void );
	IMU_6AXES_StatusTypeDef LSM6DS3_Get_Status_Free_Fall_Detection( uint8_t *status );

	IMU_6AXES_StatusTypeDef LSM6DS3_Common_Sensor_Enable(void);
	IMU_6AXES_StatusTypeDef LSM6DS3_X_Set_Axes_Status(uint8_t enableX, uint8_t enableY, uint8_t enableZ);
	IMU_6AXES_StatusTypeDef LSM6DS3_G_Set_Axes_Status(uint8_t enableX, uint8_t enableY, uint8_t enableZ);

	/**
	 * @brief  Configures LSM6DS3 interrupt lines for NUCLEO boards
	 * @param  None
	 * @retval None
	 */
	void LSM6DS3_IO_ITConfig(void)
	{
		/* To be implemented */
	}

	/**
	 * @brief  Configures LSM6DS3 I2C interface
	 * @param  None
	 * @retval IMU_6AXES_OK in case of success, an error code otherwise
	 */
	IMU_6AXES_StatusTypeDef LSM6DS3_IO_Init(void)
	{
		return IMU_6AXES_OK; /* done in constructor */
	}

	/**
	 * @brief utility function to read data from STC3115
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval IMU_6AXES_OK if ok, IMU_6AXES_ERROR if an I2C error has occured
	 */
	IMU_6AXES_StatusTypeDef LSM6DS3_IO_Read(uint8_t* pBuffer, 
					      uint8_t RegisterAddr, uint16_t NumByteToRead)
	{
		int ret = dev_i2c.i2c_read(pBuffer,
					   LSM6DS3_XG_MEMS_ADDRESS,
					   RegisterAddr,
					   NumByteToRead);
		if(ret != 0) {
			return IMU_6AXES_ERROR;
		}
		return IMU_6AXES_OK;
	}
	
	/**
	 * @brief utility function to write data to STC3115
	 * @param  pBuffer: pointer to buffer to be filled.
	 * @param  RegisterAddr: specifies internal address register to read from.
	 * @param  NumByteToWrite: number of bytes to write.
	 * @retval 0 if ok, -1 if an I2C error has occured
	 */
	IMU_6AXES_StatusTypeDef LSM6DS3_IO_Write(uint8_t* pBuffer, 
					       uint8_t RegisterAddr, uint16_t NumByteToWrite)
	{
		int ret = dev_i2c.i2c_write(pBuffer,
					    LSM6DS3_XG_MEMS_ADDRESS,
					    RegisterAddr,
					    NumByteToWrite);
		if(ret != 0) {
			return IMU_6AXES_ERROR;
		}
		return IMU_6AXES_OK;
	}
	
	/*** Instance Variables ***/
	/* IO Device */
	DevI2C &dev_i2c;
};

#endif // __LSM6DS3_CLASS_H
