/**
 ******************************************************************************
 * @file    x_nucleo_iks01a1.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    08-October-2014
 * @brief   Implementation file for the X_NUCLEO_IKS01A1 singleton class
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
#include "x_nucleo_iks01a1.h"

/* Static variables ----------------------------------------------------------*/
X_NUCLEO_IKS01A1* X_NUCLEO_IKS01A1::_instance = NULL;


/* Methods -------------------------------------------------------------------*/
/**
 * @brief  Constructor
 */
X_NUCLEO_IKS01A1::X_NUCLEO_IKS01A1(DevI2C *ext_i2c) : dev_i2c(ext_i2c),
	ht_sensor(*(new HTS221(*dev_i2c))),
	magnetometer(*(new LIS3MDL(*dev_i2c))),
	pressure_sensor(*(new LPS25H(*dev_i2c))),
	gyroscope(*(new LSM6DS0(*dev_i2c)))
{ 
}

/**
 * @brief  Get singleton instance
 * @return a pointer to the initialized singleton instance of class X_NUCLEO_IKS01A1
 * @param  (optional) ext_i2c pointer to instance of DevI2C to be used
 *         for communication on the expansion board. 
 *         Taken into account only on the very first call of this function.
 *         If not provided a new DevI2C will be created with standard
 *         configuration parameters.
 *         The used DevI2C object gets saved in instance variable dev_i2c.
 */
 X_NUCLEO_IKS01A1* X_NUCLEO_IKS01A1::Instance(DevI2C *ext_i2c) {
	if(_instance == NULL) {
		if(ext_i2c == NULL)
			ext_i2c = new DevI2C(IKS01A1_PIN_I2C_SDA, IKS01A1_PIN_I2C_SCL);

		if(ext_i2c != NULL)
			_instance = new X_NUCLEO_IKS01A1(ext_i2c);
	
		if(_instance != NULL) {
			bool ret = _instance->Init();
			if(!ret) {
				error("Failed to init X_NUCLEO_IKS01A1 expansion board!\n");
			}
		}
	}

	return _instance;
}

/**
 * @brief  Initialize the singelton HT sensor
 * @return true if initialization successful, false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_HT(void) {
	uint8_t ht_id = 0;
	HUM_TEMP_InitTypeDef InitStructure;

	/* Configure sensor */
	InitStructure.OutputDataRate = HTS221_ODR_12_5Hz;

	if(ht_sensor.Init(&InitStructure) != HUM_TEMP_OK)
		{
			return false;
		}
      
	if(ht_sensor.ReadID(&ht_id) != HUM_TEMP_OK)
		{
			return false;
		}
	
	if(ht_id != I_AM_HTS221)
		{
			return false;
		}

	return true;
}

/**
 * @brief  Initialize the singelton magnetometer
 * @return true if initialization successful, false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_MAG(void) {
	uint8_t m_id = 0;
	MAGNETO_InitTypeDef InitStructure;

	/* Configure sensor */
	InitStructure.M_FullScale = LIS3MDL_M_FS_4;
	InitStructure.M_OperatingMode = LIS3MDL_M_MD_CONTINUOUS;
	InitStructure.M_XYOperativeMode = LIS3MDL_M_OM_HP;
	InitStructure.M_OutputDataRate = LIS3MDL_M_DO_80;

	if(magnetometer.Init(&InitStructure) != MAGNETO_OK)
		{
			return false;
		}
      
	if(magnetometer.ReadID(&m_id) != MAGNETO_OK)
		{
			return false;
		}
      
	if(m_id != I_AM_LIS3MDL_M)
		{
			return false;
		}

	return true;
}

/**
 * @brief  Initialize the singelton pressure sensor
 * @return true if initialization successful, false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_PRESS(void) {
	uint8_t p_id = 0;
	PRESSURE_InitTypeDef InitStructure;
	
	/* Configure sensor */
	InitStructure.OutputDataRate = LPS25H_ODR_1Hz;
	InitStructure.BlockDataUpdate = LPS25H_BDU_CONT;
	InitStructure.DiffEnable = LPS25H_DIFF_ENABLE;
	InitStructure.SPIMode = LPS25H_SPI_SIM_3W;
	InitStructure.PressureResolution = LPS25H_P_RES_AVG_32;
	InitStructure.TemperatureResolution = LPS25H_T_RES_AVG_16;
        
	if(pressure_sensor.Init(&InitStructure) != PRESSURE_OK)
		{
			return false;
		}
            
	if(pressure_sensor.ReadID(&p_id) != PRESSURE_OK)
		{
			return false;
		}
            
	if(p_id != I_AM_LPS25H)
		{
			return false;
		}

	return true;
}

/**
 * @brief  Initialize the singelton gyroscope
 * @return true if initialization successful, false otherwise
 */
bool X_NUCLEO_IKS01A1::Init_GYRO(void) {
	IMU_6AXES_InitTypeDef InitStructure;
	uint8_t xg_id = 0;

	/* Configure sensor */
	InitStructure.G_FullScale       = LSM6DS0_G_FS_2000;
	InitStructure.G_OutputDataRate  = LSM6DS0_G_ODR_119HZ;
	InitStructure.G_X_Axis          = LSM6DS0_G_XEN_ENABLE;
	InitStructure.G_Y_Axis          = LSM6DS0_G_YEN_ENABLE;
	InitStructure.G_Z_Axis          = LSM6DS0_G_ZEN_ENABLE;

	InitStructure.X_FullScale       = LSM6DS0_XL_FS_2G;
	InitStructure.X_OutputDataRate  = LSM6DS0_XL_ODR_119HZ;
	InitStructure.X_X_Axis          = LSM6DS0_XL_XEN_ENABLE;
	InitStructure.X_Y_Axis          = LSM6DS0_XL_YEN_ENABLE;
	InitStructure.X_Z_Axis          = LSM6DS0_XL_ZEN_ENABLE;
              
	if(gyroscope.Init(&InitStructure) != IMU_6AXES_OK)
		{
			return false; 
		}
            
	if(gyroscope.ReadID(&xg_id) != IMU_6AXES_OK)
		{
			return false;
		}
            
	if(xg_id != I_AM_LSM6DS0_XG)
		{
			return false;
		}

	return true;
}
