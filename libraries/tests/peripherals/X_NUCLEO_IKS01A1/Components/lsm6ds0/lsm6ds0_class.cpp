/**
 ******************************************************************************
 * @file    lsm6ds0_class.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Implementation file for the LSM6DS0 driver class
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
#include "lsm6ds0_class.h"
#include "lsm6ds0.h"
#include "../../x_nucleo_iks01a1_targets.h"

/* Methods -------------------------------------------------------------------*/
/* betzw - based on:
           X-CUBE-MEMS1/trunk/Drivers/BSP/Components/lsm6ds0/lsm6ds0.c: revision #184,
           X-CUBE-MEMS1/trunk: revision #293
*/
/**
 * @brief  Set LSM6DS0 Initialization
 * @param  LSM6DS0_Init the configuration setting for the LSM6DS0
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_Init(IMU_6AXES_InitTypeDef *LSM6DS0_Init)
{
    uint8_t tmp1 = 0x00;

    /* Configure the low level interface ---------------------------------------*/
    if(LSM6DS0_IO_Init() != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    /******* Gyroscope init *******/

    if(LSM6DS0_IO_Read(&tmp1, LSM6DS0_XG_CTRL_REG1_G, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    /* Output Data Rate selection */
    tmp1 &= ~(LSM6DS0_G_ODR_MASK);
    tmp1 |= LSM6DS0_Init->G_OutputDataRate;

    /* Full scale selection */
    tmp1 &= ~(LSM6DS0_G_FS_MASK);
    tmp1 |= LSM6DS0_Init->G_FullScale;

    if(LSM6DS0_IO_Write(&tmp1, LSM6DS0_XG_CTRL_REG1_G, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    if(LSM6DS0_IO_Read(&tmp1, LSM6DS0_XG_CTRL_REG4, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    /* Enable X axis selection */
    tmp1 &= ~(LSM6DS0_G_XEN_MASK);
    tmp1 |= LSM6DS0_Init->G_X_Axis;

    /* Enable Y axis selection */
    tmp1 &= ~(LSM6DS0_G_YEN_MASK);
    tmp1 |= LSM6DS0_Init->G_Y_Axis;

    /* Enable Z axis selection */
    tmp1 &= ~(LSM6DS0_G_ZEN_MASK);
    tmp1 |= LSM6DS0_Init->G_Z_Axis;

    if(LSM6DS0_IO_Write(&tmp1, LSM6DS0_XG_CTRL_REG4, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    /******************************/

    /***** Accelerometer init *****/

    if(LSM6DS0_IO_Read(&tmp1, LSM6DS0_XG_CTRL_REG6_XL, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    /* Output Data Rate selection */
    tmp1 &= ~(LSM6DS0_XL_ODR_MASK);
    tmp1 |= LSM6DS0_Init->X_OutputDataRate;

    /* Full scale selection */
    tmp1 &= ~(LSM6DS0_XL_FS_MASK);
    tmp1 |= LSM6DS0_Init->X_FullScale;

    if(LSM6DS0_IO_Write(&tmp1, LSM6DS0_XG_CTRL_REG6_XL, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    if(LSM6DS0_IO_Read(&tmp1, LSM6DS0_XG_CTRL_REG5_XL, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    /* Enable X axis selection */
    tmp1 &= ~(LSM6DS0_XL_XEN_MASK);
    tmp1 |= LSM6DS0_Init->X_X_Axis;

    /* Enable Y axis selection */
    tmp1 &= ~(LSM6DS0_XL_YEN_MASK);
    tmp1 |= LSM6DS0_Init->X_Y_Axis;

    /* Enable Z axis selection */
    tmp1 &= ~(LSM6DS0_XL_ZEN_MASK);
    tmp1 |= LSM6DS0_Init->X_Z_Axis;

    if(LSM6DS0_IO_Write(&tmp1, LSM6DS0_XG_CTRL_REG5_XL, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }
    
    /* Configure interrupt lines */
    LSM6DS0_IO_ITConfig();
    
    return IMU_6AXES_OK;

    /******************************/
}


/**
 * @brief  Read ID of LSM6DS0 Accelerometer and Gyroscope
 * @param  xg_id the pointer where the ID of the device is stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_Read_XG_ID(uint8_t *xg_id)
{
    if(!xg_id)
    { 
      return IMU_6AXES_ERROR; 
    }
 
    return LSM6DS0_IO_Read(xg_id, LSM6DS0_XG_WHO_AM_I_ADDR, 1);
}


/**
 * @brief  Read raw data from LSM6DS0 Accelerometer output register
 * @param  pData the pointer where the accelerometer raw data are stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_X_GetAxesRaw(int16_t *pData)
{
    uint8_t tempReg[2] = {0,0};

    if(LSM6DS0_IO_Read(&tempReg[0], (LSM6DS0_XG_OUT_X_L_XL | LSM6DS0_I2C_MULTIPLEBYTE_CMD), 2) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    pData[0] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    if(LSM6DS0_IO_Read(&tempReg[0], (LSM6DS0_XG_OUT_Y_L_XL | LSM6DS0_I2C_MULTIPLEBYTE_CMD), 2) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    pData[1] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    if(LSM6DS0_IO_Read(&tempReg[0], (LSM6DS0_XG_OUT_Z_L_XL | LSM6DS0_I2C_MULTIPLEBYTE_CMD), 2) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    pData[2] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);
    
    return IMU_6AXES_OK;
}


/**
 * @brief  Read data from LSM6DS0 Accelerometer and calculate linear acceleration in mg
 * @param  pData the pointer where the accelerometer data are stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_X_GetAxes(int32_t *pData)
{
    uint8_t tempReg = 0x00;
    int16_t pDataRaw[3];
    float sensitivity = 0;

    if(LSM6DS0_X_GetAxesRaw(pDataRaw) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    if(LSM6DS0_IO_Read(&tempReg, LSM6DS0_XG_CTRL_REG6_XL, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    tempReg &= LSM6DS0_XL_FS_MASK;

    switch(tempReg)
    {
      case LSM6DS0_XL_FS_2G:
        sensitivity = 0.061;
        break;
      case LSM6DS0_XL_FS_4G:
        sensitivity = 0.122;
        break;
      case LSM6DS0_XL_FS_8G:
        sensitivity = 0.244;
        break;
    }

    pData[0] = (int32_t)(pDataRaw[0] * sensitivity);
    pData[1] = (int32_t)(pDataRaw[1] * sensitivity);
    pData[2] = (int32_t)(pDataRaw[2] * sensitivity);
    
    return IMU_6AXES_OK;
}


/**
 * @brief  Read raw data from LSM6DS0 Gyroscope output register
 * @param  pData the pointer where the gyroscope raw data are stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_G_GetAxesRaw(int16_t *pData)
{
    uint8_t tempReg[2] = {0,0};

    if(LSM6DS0_IO_Read(&tempReg[0], (LSM6DS0_XG_OUT_X_L_G | LSM6DS0_I2C_MULTIPLEBYTE_CMD), 2) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    pData[0] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    if(LSM6DS0_IO_Read(&tempReg[0], (LSM6DS0_XG_OUT_Y_L_G | LSM6DS0_I2C_MULTIPLEBYTE_CMD), 2) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    pData[1] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    if(LSM6DS0_IO_Read(&tempReg[0], (LSM6DS0_XG_OUT_Z_L_G | LSM6DS0_I2C_MULTIPLEBYTE_CMD), 2) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    pData[2] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);
    
    return IMU_6AXES_OK;
}


/**
 * @brief  Read data from LSM6DS0 Gyroscope and calculate angular rate in mdps
 * @param  pData the pointer where the gyroscope data are stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_G_GetAxes(int32_t *pData)
{
    uint8_t tempReg = 0x00;
    int16_t pDataRaw[3];
    float sensitivity = 0;

    if(LSM6DS0_G_GetAxesRaw(pDataRaw) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    if(LSM6DS0_IO_Read(&tempReg, LSM6DS0_XG_CTRL_REG1_G, 1) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }

    tempReg &= LSM6DS0_G_FS_MASK;

    switch(tempReg)
    {
      case LSM6DS0_G_FS_245:
        sensitivity = 8.75;
        break;
      case LSM6DS0_G_FS_500:
        sensitivity = 17.50;
        break;
      case LSM6DS0_G_FS_2000:
        sensitivity = 70;
        break;
    }

    pData[0] = (int32_t)(pDataRaw[0] * sensitivity);
    pData[1] = (int32_t)(pDataRaw[1] * sensitivity);
    pData[2] = (int32_t)(pDataRaw[2] * sensitivity);
    
    return IMU_6AXES_OK;
}

/**
 * @brief  Read Accelero Sensitivity
 * @param  pfData the pointer where the accelerometer sensitivity is stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
 */
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_X_GetSensitivity( float *pfData )
{
    /*Here we have to add the check if the parameters are valid*/
    uint8_t tempReg = 0x00;
    
    if(LSM6DS0_IO_Read( &tempReg, LSM6DS0_XG_CTRL_REG6_XL, 1 ) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }
    
    tempReg &= LSM6DS0_XL_FS_MASK;
    
    switch( tempReg )
    {
      case LSM6DS0_XL_FS_2G:
        *pfData = 0.061;
        break;
      case LSM6DS0_XL_FS_4G:
        *pfData = 0.122;
        break;
      case LSM6DS0_XL_FS_8G:
        *pfData = 0.244;
        break;
      default:
        break;
    }
    
    return IMU_6AXES_OK;
}



/**
 * @brief  Read Gyro Sensitivity
 * @param  pfData the pointer where the gyroscope sensitivity is stored
 * @retval IMU_6AXES_OK in case of success, an error code otherwise
*/
IMU_6AXES_StatusTypeDef LSM6DS0::LSM6DS0_G_GetSensitivity( float *pfData )
{
    /*Here we have to add the check if the parameters are valid*/
    uint8_t tempReg = 0x00;
    
    if(LSM6DS0_IO_Read( &tempReg, LSM6DS0_XG_CTRL_REG1_G, 1 ) != IMU_6AXES_OK)
    {
      return IMU_6AXES_ERROR;
    }
    
    tempReg &= LSM6DS0_G_FS_MASK;
    
    switch( tempReg )
    {
      case LSM6DS0_G_FS_245:
        *pfData = 8.75;
        break;
      case LSM6DS0_G_FS_500:
        *pfData = 17.50;
        break;
      case LSM6DS0_G_FS_2000:
        *pfData = 70;
        break;
      default:
        break;
    }
    
    return IMU_6AXES_OK;
}
