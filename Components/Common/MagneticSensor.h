/**
 ******************************************************************************
 * @file    MagneticSensor.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    13-April-2015
 * @brief   This file contains the abstract class describing in general
 *          the interfaces of a magnetometer
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

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __MAGNETIC_SENSOR_CLASS_H
#define __MAGNETIC_SENSOR_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Classes  ------------------------------------------------------------------*/
/** An abstract class for a magnetometer
 */
class MagneticSensor
{
 public:
	/**
	 * @brief       Initialization of magnetometer
	 * @param[out]  Pointer to device specific initalization structure
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int Init(void*) = 0;
	
	/**
	 * @brief       Get ID of magnetometer
	 * @param[out]  Pointer to where to store the ID to
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int ReadID(uint8_t*) = 0;

	/**
	 * @brief       Get current magnetometer magnetic X/Y/Z-axes values 
	 *              in standard data units [mgauss]
	 * @param[out]  Pointer to where to store magnetic values to.
	 *              Pointer must point to an array of (at least) three elements, where:
	 *              ptr[0] corresponds to X-axis
	 *              ptr[1] corresponds to Y-axis
	 *              ptr[2] corresponds to Z-axis
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int Get_M_Axes(int32_t*) = 0;

	/**
	 * @brief       Get current magnetometer raw data X/Y/Z-axes values 
	 *              in device sepcific LSB units
	 * @param[out]  Pointer to where to store magnetometer raw data to.
	 *              Pointer must point to an array of (at least) three elements, where:
	 *              ptr[0] corresponds to X-axis
	 *              ptr[1] corresponds to Y-axis
	 *              ptr[2] corresponds to Z-axis
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int Get_M_AxesRaw(int16_t *) = 0;
};

#endif /* __MAGNETIC_SENSOR_CLASS_H */
