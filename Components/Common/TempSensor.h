/**
 ******************************************************************************
 * @file    TempSensor.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    13-April-2015
 * @brief   This file contains the abstract class describing in general
 *          the interfaces of a temperature sensor
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
#ifndef __TEMP_SENSOR_CLASS_H
#define __TEMP_SENSOR_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Classes  ------------------------------------------------------------------*/
/** An abstract class for Temperature sensors
 */
class TempSensor
{
 public:
	/**
	 * @brief       Initialization of temperature sensor
	 * @param[out]  Pointer to device specific initalization structure
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int Init(void*) = 0;

	/**
	 * @brief       Enter sensor shutdown mode
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int PowerOff(void) = 0;
	
	/**
	 * @brief       Get ID of temperature sensor
	 * @param[out]  Pointer to where to store the ID to
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int ReadID(uint8_t*) = 0;

	/**
	 * @brief       Reset sensor
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int Reset(void) = 0;

	/**
	 * @brief       Get current temperature [°C]
	 * @param[out]  Pointer to where to store temperature to
	 * @retval      0 in case of success, an error code otherwise
	 */
	virtual int GetTemperature(float*) = 0;

	virtual void ConfigIT(uint16_t) = 0;
	virtual void EnableIT(uint8_t) = 0;
	virtual void DisableIT(uint8_t) = 0;
	virtual uint8_t ITStatus(uint16_t, uint16_t) = 0;
	virtual void ClearIT(uint16_t, uint16_t) = 0;

	/**
	 * @brief       Attach a function to be called when an interrupt occurs
	 * @param[in]   fptr A pointer to a void function, or 0 to set as none
	 */
	virtual void AttachIT(void (*fptr)(void)) = 0;
};

#endif /* __TEMP_SENSOR_CLASS_H */
