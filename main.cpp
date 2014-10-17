/* Copyright (C) STMicroelectronics Srl 2014 */
/*
 * @(#)main.cpp
 *
 * Example application for using the X_NUCLEO_IKC01A1 battery
 * mangement expansion board.
 *
 */

#include "mbed.h"
#include "x_nucleo_ikc01a1.h"

#include <Ticker.h>

namespace {
	const int MS_INTERVALS = 1000;
}

static X_NUCLEO_IKC01A1 *battery_expansion_board = X_NUCLEO_IKC01A1::Instance();

static Ticker timer;
static DigitalOut led(LED1);
static DigitalIn button(USER_BUTTON);

static volatile bool irq_triggered = false;

/* return string describing charger state */
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

/* Everything is happening here 
   (and above all in "normal" context, i.e. not in IRQ context)
*/
static void main_cycle(void) {
	led = !led; // Blink
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
   time based cyclic execution
*/
int main()
{
	printf("--- Starting new run ---\n");
	timer.attach_us(timer_irq, MS_INTERVALS * 1000);

	while (true) {
		__disable_irq();
		if(irq_triggered) {
			irq_triggered = false;
			__enable_irq();
			main_cycle();
		} else {
			__enable_irq();
			__WFI();
		}
	}
}
