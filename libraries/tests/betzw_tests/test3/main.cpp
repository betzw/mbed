/**
 ******************************************************************************
 * @file    main.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-August-2015
 * @brief   Simple Example application for using the X_NUCLEO_IKS01A1 
 *          MEMS Inertial & Environmental Sensor Nucleo expansion board.
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
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/ 

#include "mbed.h"
#include "x_nucleo_iks01a1.h"

/* Instantiate the expansion board */
static X_NUCLEO_IKS01A1 *mems_expansion_board = X_NUCLEO_IKS01A1::Instance();

/* Retrieve the composing elements of the expansion board */
static GyroSensor *gyroscope = mems_expansion_board->GetGyroscope();
static MotionSensor *accelerometer = mems_expansion_board->GetAccelerometer();
static MagneticSensor *magnetometer = mems_expansion_board->magnetometer;
static HumiditySensor *humidity_sensor = mems_expansion_board->ht_sensor;
static PressureSensor *pressure_sensor = mems_expansion_board->pressure_sensor;
static TempSensor *temp_sensor1 = mems_expansion_board->ht_sensor;
static TempSensor *temp_sensor2 = mems_expansion_board->pressure_sensor;

/* Helper function for printing floats & doubles */
static char *printDouble(char* str, double v, int decimalDigits=2)
{
  int i = 1;
  int intPart, fractPart;
  int len;
  char *ptr;

  /* prepare decimal digits multiplicator */
  for (;decimalDigits!=0; i*=10, decimalDigits--);

  /* calculate integer & fractinal parts */
  intPart = (int)v;
  fractPart = (int)((v-(double)(int)v)*i);

  /* fill in integer part */
  sprintf(str, "%i.", intPart);

  /* prepare fill in of fractional part */
  len = strlen(str);
  ptr = &str[len];

  /* fill in leading fractional zeros */
  for (i/=10;i>1; i/=10, ptr++) {
    if(fractPart >= i) break;
    *ptr = '0';
  }

  /* fill in (rest of) fractional part */
  sprintf(ptr, "%i", fractPart);

  return str;
}


/* Simple main function */
int main() {
  uint8_t id;
  float value1, value2;
  char buffer1[32], buffer2[32];
  int32_t axes[3];
  
  printf("\r\n--- Starting new run ---\r\n");

  humidity_sensor->ReadID(&id);
  printf("HTS221  humidity & temperature    = 0x%X\r\n", id);
  pressure_sensor->ReadID(&id);
  printf("LPS25H  pressure & temperature    = 0x%X\r\n", id);
  magnetometer->ReadID(&id);
  printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
  gyroscope->ReadID(&id);
  printf("LSM6DS0 accelerometer & gyroscope = 0x%X\r\n", id);
  
  wait(3);
 
  while(1) {
    printf("\r\n");

    temp_sensor1->GetTemperature(&value1);
    humidity_sensor->GetHumidity(&value2);
    printf("HTS221: %7s°C,  %s%%\r\n", printDouble(buffer1, value1), printDouble(buffer2, value2));
    
    temp_sensor2->GetFahrenheit(&value1);
    pressure_sensor->GetPressure(&value2);
    printf("LPS25H: %7s°F, %smbar\r\n", printDouble(buffer1, value1), printDouble(buffer2, value2));

    printf("---\r\n");

    magnetometer->Get_M_Axes(axes);
    printf("LIS3MDL [mgauss]: %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

    accelerometer->Get_X_Axes(axes);
    printf("LSM6DS0 [mg]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

    gyroscope->Get_G_Axes(axes);
    printf("LSM6DS0 [mdps]:   %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

    wait(1.5);
  }
}
