/**
 ******************************************************************************
 * @file    main.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Example application for using the X_NUCLEO_IKS01A1 
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

/**
 * @mainpage X_NUCLEO_IKS01A1 MEMS Inertial & Environmental Sensor Nucleo Expansion Board Firmware Package
 *
 * <b>Introduction</b>
 *
 * This firmware package includes Components Device Drivers, Board Support Package
 * and example application for STMicroelectronics X_NUCLEO_IKS01A1 MEMS Inertial & Environmental Nucleo
 * Expansion Board
 * 
 * <b>Example Application</b>
 *
 */


/*** Includes ----------------------------------------------------------------- ***/
#include "mbed.h"
#include "x_nucleo_iks01a1.h"

#include <Ticker.h>


/*** Constants ---------------------------------------------------------------- ***/
namespace {
	const int MS_INTERVALS = 1000;
}


/*** Macros ------------------------------------------------------------------- ***/
#define APP_LOOP_PERIOD 1300 // in ms

#if defined(TARGET_STM)
#define LED_OFF (0)
#else
#define LED_OFF (1)
#endif
#define LED_ON  (!LED_OFF)


/*** Typedefs ----------------------------------------------------------------- ***/
typedef struct {
    int32_t AXIS_X;
    int32_t AXIS_Y;
    int32_t AXIS_Z;
} AxesRaw_TypeDef;


/*** Static variables --------------------------------------------------------- ***/
#ifdef DBG_MCU
/* betzw: enable debugging while using sleep modes */
#include "DbgMCU.h"
static DbgMCU enable_dbg;
#endif // DBG_MCU

static X_NUCLEO_IKS01A1 *mems_expansion_board = X_NUCLEO_IKS01A1::Instance();
static GyroSensor &gyroscope = mems_expansion_board->gyroscope;
static MotionSensor &accelerometer = mems_expansion_board->gyroscope;
static MagneticSensor &magnetometer = mems_expansion_board->magnetometer;
static HumiditySensor &humidity_sensor = mems_expansion_board->ht_sensor;;
static PressureSensor &pressure_sensor = mems_expansion_board->pressure_sensor;
static TempSensor &temp_sensor1 = mems_expansion_board->ht_sensor;
static TempSensor &temp_sensor2 = mems_expansion_board->pressure_sensor;

static Ticker ticker;
static volatile bool timer_irq_triggered = false;
static DigitalOut myled(LED1, LED_OFF);


/*** Helper Functions (1/2) ------------------------------------------------------------ ***/


/*** Interrupt Handler Top-Halves ------------------------------------------------------ ***/
/* Called in interrupt context, therefore just set a trigger variable */
static void timer_irq(void) {
	timer_irq_triggered = true;
}


/*** Interrupt Handler Bottom-Halves ------------------------------------------------- ***/


/*** Helper Functions (2/2) ------------------------------------------------------------ ***/
/* print floats & doubles */
static char *printDouble(char* str, double v, int decimalDigits=2)
{
  int i = 1;
  int intPart, fractPart;

  for (;decimalDigits!=0; i*=10, decimalDigits--);
  intPart = (int)v;
  fractPart = (int)((v-(double)(int)v)*i);
  sprintf(str, "%i.%i", intPart, fractPart);

  return str;
}

/* Initialization function */
static void init(void) {
	uint8_t hts221_id_hum;
	uint8_t hts221_id_temp;

	/* Determine ID of Humidity & Temperature Sensor */
	humidity_sensor.ReadID(&hts221_id_hum);
	temp_sensor1.ReadID(&hts221_id_temp);
    	printf("HTS221_ID (Humidity)    = 0x%x (%u)\n", hts221_id_hum, hts221_id_hum);
    	printf("HTS221_ID (Temperature) = 0x%x (%u)\n", hts221_id_temp, hts221_id_temp);
}

/* Main cycle function */
static void main_cycle(void) {
	float TEMPERATURE_Value;
	float HUMIDITY_Value;
	float PRESSURE_Value;
	float PRESSURE_Temp_Value;
	AxesRaw_TypeDef MAG_Value;
	AxesRaw_TypeDef ACC_Value;
	AxesRaw_TypeDef GYR_Value;
	char buffer1[32];
	char buffer2[32];
	char buffer3[32];
	char buffer4[32];

	/* Switch LED On */
	myled = LED_ON;
	printf("===\n");

	/* Determine Environmental Values */
        temp_sensor1.GetTemperature(&TEMPERATURE_Value);
        humidity_sensor.GetHumidity(&HUMIDITY_Value);
        pressure_sensor.GetPressure(&PRESSURE_Value);
        temp_sensor2.GetFahrenheit(&PRESSURE_Temp_Value);
        magnetometer.Get_M_Axes((int32_t *)&MAG_Value);
        accelerometer.Get_X_Axes((int32_t *)&ACC_Value);
        gyroscope.Get_G_Axes((int32_t *)&GYR_Value);

	/* Print Values Out */
        printf("                      X         Y         Z\n"); 
        printf("MAG [mgauss]: %9ld %9ld %9ld\n", 
	       MAG_Value.AXIS_X, MAG_Value.AXIS_Y, MAG_Value.AXIS_Z);
        printf("ACC [mg]:     %9ld %9ld %9ld\n", 
	       ACC_Value.AXIS_X, ACC_Value.AXIS_Y, ACC_Value.AXIS_Z);
        printf("GYR [mdps]:   %9ld %9ld %9ld\n", 
	       GYR_Value.AXIS_X, GYR_Value.AXIS_Y, GYR_Value.AXIS_Z);
        printf("---\nTEMP | HUMIDITY: %s°C | %s%%\nTEMP | PRESSURE: %s°F | %smbar\n", 
	       printDouble(buffer1, TEMPERATURE_Value), 
	       printDouble(buffer2, HUMIDITY_Value), 
	       printDouble(buffer4, PRESSURE_Temp_Value),
	       printDouble(buffer3, PRESSURE_Value));
	
	/* Switch LED Off */
	myled = LED_OFF;
}


/*** Main function ------------------------------------------------------------- ***/
/* Generic main function/loop for enabling WFI in case of 
   interrupt based cyclic execution
*/
int main()
{
	/* Start & initialize */
	printf("\n--- Starting new run ---\n");
	init();

	/* Start timer irq */
	ticker.attach_us(timer_irq, MS_INTERVALS * APP_LOOP_PERIOD);

	while (true) {
		__disable_irq();
		if(timer_irq_triggered) {
			timer_irq_triggered = false;
			__enable_irq();
			main_cycle();
		} else {
			__WFI();
			__enable_irq(); /* do NOT enable irqs before WFI to avoid 
					   opening a window in which you can loose
					   irq arrivals before going into WFI */
		}
	}
}
