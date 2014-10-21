/**
******************************************************************************
* @file    main.cpp
* @author  AST / EST
* @version V0.0.1
* @date    08-October-2014
* @brief   Example application for using the X_NUCLEO_IKC01A1 battery
*          mangement expansion board.
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

/**
 * @mainpage X_NUCLEO_IKC01A1 Energy Management Expansion Board firmware package
 *
 * <b>Introduction</b>
 *
 * This firmware package includes Components Device Drivers, Board Support Package
 * and example application for STMicroelectronics X_NUCLEO_IKC01A1 Energy Management
 * Expansion Board
 * 
 * <b>Example Application</b>
 *
 * Example application allows the user to interact with the Expansion Board via UART.
 * Launch a terminal application and set the UART port to 9600 bps, 8 bit, No Parity,
 * 1 stop bit. Example application shows how to configure and operate the three main
 * devices that the Expansion Board features:
 *   - STC3115 Gas Gauge for 3V7 LiPo battery
 *   - M41T62 Real Time Clock
 *   - L6924D Battery charger
 *
 * If the Expansion Board is powered for the first time, the user is prompted to insert
 * current Time of Day information in order to configure onboard Real Time Clock. If the
 * battery is not removed for more than 10 seconds, the Time of Day is retained by the
 * RTC and user is not prompted to set Time of Day any longer.
 * Once application starts, the following status information about the three devices
 * is reported on the terminal every two seconds:
 *   - Time of Day
 *   - Battery Voltage
 *   - Instantaneous current sinked by application
 *   - Battery State of Charge
 *   - Battery Residual Charge
 *   - Battery Presence
 *   - Gas Gauge Alarm status bits
 *   - Battery Charge status
 *
 * Application console output example: <br>
 * 17:40:22 Vbat: 4110mV, SoC=94% I=-15mA, C=15, P=1 A=1, Charge state: invalid - !discharging, vbatTh=4100mV, socTh=98%, alm=5sec Gas Gauge alarm set to 'b01' - Press button to clear alarm <br>
 * 17:40:24 Vbat: 4112mV, SoC=94% I=-4mA, C=15, P=1 A=1, Charge state: invalid - !discharging, vbatTh=4100mV, socTh=98%, alm=5sec Gas Gauge alarm set to 'b01' - Press button to clear alarm <br>
 * 17:40:26 Vbat: 4112mV, SoC=94% I=-4mA, C=15, P=1 A=1, Charge state: invalid - !discharging, vbatTh=4100mV, socTh=98%, alm=5sec (*) Gas Gauge alarm set to 'b01' - Press button to clear alarm <br>
 *
 *
 * If the user presses button B1 on Nucleo board, the following settings can be
 * configured:
 *   - State of Charge alarm threshold
 *   - Voltage alarm threshold
 *   - RTC alarm period
 *   - Enable/Disable battery forced discharge
 *
 * Gas Gauge and RTC alarms are reported on the cosole. RTC alarms are automatically
 * cleared, whereas Gas Gauge alarms are cleared by pressing the B1 button again
 */

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "userinterface.h"
#include "x_nucleo_ikc01a1.h"

#include <Ticker.h>

/* Constants -----------------------------------------------------------------*/
namespace {
	const int MS_INTERVALS = 2000;
}

/* Macros --------------------------------------------------------------------*/
#define APP_LOOP_PERIOD 1000 // in ms

/* Static variables ----------------------------------------------------------*/
static X_NUCLEO_IKC01A1 *battery_expansion_board = X_NUCLEO_IKC01A1::Instance();

static Ticker timer;
static InterruptIn button(USER_BUTTON);

static volatile bool timer_irq_triggered = false;
static volatile bool button_irq_triggered = false;
static volatile bool gg_irq_triggered = false;
static volatile bool rtc_irq_triggered = false;

static int socThreshold = 0;
static int voltageThreshold = 0;
static int periodicRtcAlarm = 0;
static int alarmCounter = 0;
static int dischargingEnabled = 0;

static int main_rtc_irq_triggered = 0;

/* Functions -----------------------------------------------------------------*/
/* Returns string describing charger state */
static const char *getChargerCondition(void) {
	charger_conditions_t state = 
		battery_expansion_board->charger.GetState();
	
	switch (state) {
	case CHARGER_CHARGE_IN_PROGRESS:
		return "charging";		
	case CHARGER_CHARGE_DONE:
		return "done";
	case CHARGER_IN_STAND_BY:
		return "stand by";
	default:
		break;
	}
	return "invalid";
}

/* Called in interrupt context, therefore just set a trigger variable */
static void timer_irq(void) {
	timer_irq_triggered = true;
}

/* Called in interrupt context, therefore just set a trigger variable */
static void button_irq(void) {
	button_irq_triggered = true;
	button.disable_irq();
}

/* Called in interrupt context, therefore just set a trigger variable */
static void rtc_irq(void) {
	rtc_irq_triggered = true;
	battery_expansion_board->rtc.disable_irq();
}

/* Initialization function */
static void init(void) {
	rtc_time_t time;
	int ret;

	/* Set irq handler for button */
	button.fall(button_irq);

	/* Check whether RTC needs to be configured */
	ret = battery_expansion_board->rtc.IsTimeOfDayValid();
	if(ret < 0) error("I2C error!\n");

	if(ret  != 1) {
		cuiGetTime(&time);
		battery_expansion_board->rtc.SetTime(&time);
	}
	
	printf("\r\nSTMicroelectronics Energy Management Expansion Board demo application\r\n");
	
	battery_expansion_board->rtc.GetTime(&time);
	printf("Today is %s, %02i %s %02i\r\n", 
	       battery_expansion_board->rtc.GetWeekName(time.tm_wday), time.tm_mday,
	       battery_expansion_board->rtc.GetMonthName(time.tm_mon), time.tm_year);
	
	/* Atach RTC interrupt handler */
	battery_expansion_board->rtc.attach_irq(rtc_irq);
}

/** @brief Asks user application settings: SoC and Voltage thresholds, RTC Alarm, Discharge on/off
 * @param None
 * @retval None
 */
static void setUserInputParams(void)
{
	/* Asks user RTC alarm, SoC and voltage thresholds */
	getUserInputParams(&socThreshold, &voltageThreshold, &periodicRtcAlarm, &dischargingEnabled);
		
#if 0 // betzw: TODO
	/* This is needed, otherwise alarms can't be re-scheduled without resetting application */
	BSP_GG_AlarmStop();
	
	/* Clear any pending IT */
	BSP_GG_AlarmClear();
	
	/* Set thresholds */
	BSP_GG_SetVoltageThreshold(voltageThreshold);
	BSP_GG_SetSOCThreshold(socThreshold);
	
	/* Enable Alarm */
	BSP_GG_AlarmSet();
#endif // 0
	
	/* Set periodic RTC Alarm */
	if(periodicRtcAlarm == 0) { // clear alarm & Irq
		battery_expansion_board->rtc.ClearAlarm();
		battery_expansion_board->rtc.ClearIrq();
		battery_expansion_board->rtc.disable_irq();
	} else { // set alarm
		rtc_alarm_t alm;

		memset(&alm, 0, sizeof(alm));
		alm.alm_repeat_mode = ONCE_PER_SEC;
		
		battery_expansion_board->rtc.SetAlarm(&alm);
		battery_expansion_board->rtc.enable_irq();
	}
	
	/* Enable/disable discharging */
	battery_expansion_board->charger.discharge = dischargingEnabled;
}

/* Handle button irq
   (here we are in "normal" context, i.e. not in IRQ context)
*/
static void handle_button_irq(void) {
	if(periodicRtcAlarm || socThreshold || voltageThreshold || dischargingEnabled) {
		/* Clear RTC alarm */
		if(periodicRtcAlarm) {
			battery_expansion_board->rtc.ClearAlarm();
			battery_expansion_board->rtc.ClearIrq();
			battery_expansion_board->rtc.disable_irq();

			/* Reset variables */
			periodicRtcAlarm = alarmCounter = 0;
			
			printf("RTC periodic alarm cleared\r\n");
		}
		
		/* Clear GasGauge alarms */
		if(socThreshold || voltageThreshold){
#if 0 // betzw: TODO
			BSP_GG_AlarmClear();
#endif // 0
			/* Reset variables */
			socThreshold = voltageThreshold = 0;
			
			printf("Gas Gauge alarms cleared\r\n");
		}
		
		/* Clear discharging */
		if(dischargingEnabled != 0) {
			battery_expansion_board->charger.discharge = dischargingEnabled = 0;
			printf("Discharging cleared\r\n");
		}
	} else {
		setUserInputParams();
	}

	/* Re-enable button irq */
	button.enable_irq();
}

/* Handle RTC alarm
   (here we are in "normal" context, i.e. not in IRQ context)
*/
static void handle_rtc_alarm(void) {
	alarmCounter++;
	if(alarmCounter >= periodicRtcAlarm) {
		main_rtc_irq_triggered = 1;
		alarmCounter = 0;
	}
	battery_expansion_board->rtc.ClearIrq();
	battery_expansion_board->rtc.enable_irq();
}

/* Everything is happening here 
   (and above all in "normal" context, i.e. not in IRQ context)
*/
static void main_cycle(void) {
	rtc_time_t time;
	
	/* Handle potential interrupts */
	// betzw: TODO

	/* Get Time of Day */
	battery_expansion_board->rtc.GetTime(&time);
	printf("%02i:%02i:%02i ", time.tm_hour, time.tm_min, time.tm_sec);
	
	/* Update Gas Gauge driver */
	// betzw: TODO
	
	/* Print out status */
	printf("Charge state: %s - %s, alm=%isec %s",
	       getChargerCondition(),
	       (battery_expansion_board->charger.discharge == 0) ? 
	       "!discharging" : "discharging",
	       periodicRtcAlarm, (main_rtc_irq_triggered ?  "(*)" : ""));
	
	/* reset handled information */
	main_rtc_irq_triggered = 0;
	
	printf("\n");
}


/* Main function --------------------------------------------------------------*/
/* Generic main function for enabling WFI in case of 
   interrupt based cyclic execution
*/
int main()
{
	/* Start & initialize */
	printf("\n--- Starting new run ---\n");
	init();

	/* Start timer irq */
	timer.attach_us(timer_irq, MS_INTERVALS * APP_LOOP_PERIOD);

	while (true) {
		__disable_irq();
		if(timer_irq_triggered) {
			timer_irq_triggered = false;
			__enable_irq();
			main_cycle();
		} else if(button_irq_triggered) {
			button_irq_triggered = false;
			__enable_irq();
			handle_button_irq();
		} else if(rtc_irq_triggered) {
			rtc_irq_triggered = false;
			__enable_irq();
			handle_rtc_alarm();
		} else {
			__WFI();
			__enable_irq(); /* do NOT enable irqs before WFI to avoid 
					   opening a window in which you can loose
					   irq arrivals before going into WFI */
		}
	}
}
