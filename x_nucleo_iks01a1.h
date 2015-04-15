/**
 ******************************************************************************
 * @file    x_nucleo_iks01a1.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    13-April-2015
 * @brief   Header file for class X_NUCLEO_IKS01A1 representing a X-NUCLEO-IKS01A1
 *          expansion board
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
#ifndef __X_NUCLEO_IKS01A1_H
#define __X_NUCLEO_IKS01A1_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_iks01a1_targets.h"
#include "DevI2C.h"
#include "Common/HumTemp.h"
#include "Common/Magneto.h"
#include "Common/Pressure.h"
#include "Common/Imu6Axes.h"

/* Classes -------------------------------------------------------------------*/
/** Class X_NUCLEO_IKS01A1 is intended to represent the MEMS Inertial & Environmental 
 *  Nucleo Expansion Board with the same name.
 *
 *  The expansion board is featuring basically four IPs:\n
 *  -# a HTS221 Relative Humidity and Temperature Sensor\n
 *  -# a LIS3MDL 3-Axis Magnetometer\n
 *  -# a LPS25H MEMS Pressure Sensor\n
 *  -# and a LSM6DS0 3D Acceleromenter and 3D Gyroscope\n
 *
 * It is intentionally implemented as a singleton because only one
 * X_NUCLEO_IKS01A1 at a time might be deployed in a HW component stack.\n
 * In order to get the singleton instance you have to call class method `Instance()`, 
 * e.g.:
 * @code
 * // Inertial & Environmental expansion board singleton instance
 * static X_NUCLEO_IKS01A1 *<TODO>_expansion_board = X_NUCLEO_IKS01A1::Instance();
 * @endcode
 */
class X_NUCLEO_IKS01A1
{
 protected:
	X_NUCLEO_IKS01A1(DevI2C *ext_i2c);

	bool Init(void) {
		return (Init_HT() &&
			Init_MAG() &&
			Init_PRESS() &&
			Init_GYRO());
	}

	bool Init_HT(void);
	bool Init_MAG(void);
	bool Init_PRESS(void);
	bool Init_GYRO(void);

 public:
	static X_NUCLEO_IKS01A1* Instance(DevI2C *ext_i2c = NULL);

	DevI2C *dev_i2c;

	HumTemp &ht_sensor;
	Magneto &magnetometer;
	Pressure &pressure_sensor;
	Imu6Axes &gyroscope;

 private:
	static X_NUCLEO_IKS01A1 *_instance;
};

#endif /* __X_NUCLEO_IKS01A1_H */
