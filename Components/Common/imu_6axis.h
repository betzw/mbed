/**
 ******************************************************************************
 * @file    imu_6axis.h
 * @author  MEMS Application Team
 * @version V1.2.0
 * @date    28-January-2015
 * @brief   This header file contains the functions prototypes for the 
 *          accelerometer and gyroscope driver.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IMU_6AXIS_H
#define __IMU_6AXIS_H

#ifdef __cplusplus
extern "C" {
#endif

    /* Includes ------------------------------------------------------------------*/
#include <stdint.h> 

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */
    
/** @addtogroup IMU_6AXIS
  * @{
  */

/** @defgroup IMU_6AXIS_Exported_Types
  * @{
  */
    
/**
 * @brief  IMU_6AXIS init structure definition
 */
typedef struct
{
    uint8_t G_OutputDataRate;
    uint8_t G_FullScale;
    uint8_t G_X_Axis;
    uint8_t G_Y_Axis;
    uint8_t G_Z_Axis;
    uint8_t X_OutputDataRate;
    uint8_t X_FullScale;
    uint8_t X_X_Axis;
    uint8_t X_Y_Axis;
    uint8_t X_Z_Axis;
}IMU_6AXIS_InitTypeDef;

/**
 * @brief  IMU_6AXIS status enumerator definition
 */
typedef enum {
    IMU_6AXIS_OK = 0,
    IMU_6AXIS_ERROR = 1,
    IMU_6AXIS_TIMEOUT = 2,
    IMU_6AXIS_NOT_IMPLEMENTED = 3
} IMU_6AXIS_StatusTypeDef;

/**
 * @brief  IMU_6AXIS component id enumerator definition
 */
typedef enum {
    IMU_6AXIS_NONE_COMPONENT = 0,
    IMU_6AXIS_LSM6DS0_COMPONENT = 1,
    IMU_6AXIS_LSM6DS3_DIL24_COMPONENT = 2
} IMU_6AXIS_ComponentTypeDef;

/**
 * @brief  IMU_6AXIS driver extended structure definition
 */
typedef struct {
    IMU_6AXIS_ComponentTypeDef id; /* This id must be unique for each component belonging to this class that wants to extend common class */
    void *pData; /* This pointer is specific for each component */
}IMU_6AXIS_DrvExtTypeDef;

/**
 * @brief  IMU_6AXIS driver structure definition
 */
typedef struct {
    IMU_6AXIS_StatusTypeDef                  (*Init)(IMU_6AXIS_InitTypeDef *);
    IMU_6AXIS_StatusTypeDef                  (*Read_XG_ID)(uint8_t *);
    IMU_6AXIS_StatusTypeDef                  (*Get_X_Axes)(int32_t *);
    IMU_6AXIS_StatusTypeDef                  (*Get_G_Axes)(int32_t *);
    IMU_6AXIS_StatusTypeDef                  (*Get_X_Sensitivity) (float *);
    IMU_6AXIS_StatusTypeDef                  (*Get_G_Sensitivity) (float *);
    IMU_6AXIS_DrvExtTypeDef                  *extData;
}IMU_6AXIS_DrvTypeDef;

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __IMU_6AXIS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
