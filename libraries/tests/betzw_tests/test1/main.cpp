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

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1.h"

#include <Ticker.h>

/* Constants -----------------------------------------------------------------*/
namespace {
	const int MS_INTERVALS = 1000;
}


/* Static variables ----------------------------------------------------------*/
static X_NUCLEO_IKC01A1 *battery_expansion_board = X_NUCLEO_IKC01A1::Instance();

static Ticker timer;
static DigitalIn button(USER_BUTTON);

static volatile bool irq_triggered = false;

static rtc_time_t current_time;

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
	irq_triggered = true;
}

/* Initialization function */
static int init(void) {
	int ret;

	/* Check whether RTC needs to be configured */
	if((ret = battery_expansion_board->rtc.IsTimeOfDayValid()) != 1) {
		/* betzw: TODO */
#if 0
		cuiGetTime(&current_time);
		BSP_RTC_SetTime(&current_time);
#else
		printf("Time is not valid (%d)\n", ret);
#endif
	}
	
	printf("\r\nSTMicroelectronics Energy Management Expansion Board demo application\r\n");
	
	battery_expansion_board->rtc.GetTime(&current_time);
	printf("Today is %s, %02i %s %02i\r\n", 
	       battery_expansion_board->rtc.GetWeekName(current_time.tm_wday), current_time.tm_mday,
	       battery_expansion_board->rtc.GetMonthName(current_time.tm_mon), current_time.tm_year);
}

/* Everything is happening here 
   (and above all in "normal" context, i.e. not in IRQ context)
*/
static void main_cycle(void) {
	if(button == 0) { // Switch discharging state
		printf("Switching discharging state\n");
		battery_expansion_board->charger.discharge =
			!battery_expansion_board->charger.discharge;
	}
	printf("Charge state: %s - %s\n",
	       getChargerCondition(),
	       battery_expansion_board->charger.discharge ? "discharging" : "!discharging");
}

/* Generic main function for enabling WFI in case of 
   interrupt based cyclic execution
*/
int main()
{
	int ret;

	/* Start & initialize */
	printf("--- Starting new run ---\n");
	ret = init();

	/* Start timer irq */
	timer.attach_us(timer_irq, MS_INTERVALS * 1000);

	while (true) {
		__disable_irq();
		if(irq_triggered) {
			irq_triggered = false;
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
