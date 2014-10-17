/* Copyright (C) STMicroelectronics Srl 2014 */
/*
 * @(#)x_nucleo_ikc01a1_charger.h
 *
 */

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __X_NUCLEO_IKC01A1_CHARGER_H
#define __X_NUCLEO_IKC01A1_CHARGER_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1_targets.h"

/* Typedefs ------------------------------------------------------------------*/
typedef enum charger_conditions {
	CHARGER_CHARGE_IN_PROGRESS,
	CHARGER_CHARGE_DONE,
	CHARGER_IN_STAND_BY,
	CHARGER_INVALID_STATE
} charger_conditions_t;

/* Classes  ------------------------------------------------------------------*/
class L6924D {
 public:
        L6924D(void) :  discharge(CHARGER_PIN_DISCHARGE), st1(CHARGER_PIN_ST1), st2(CHARGER_PIN_ST2) {
		discharge = 0;	
	}

	charger_conditions_t GetState(void);

	DigitalOut discharge;

 private:
	DigitalIn st1;
	DigitalIn st2;
};

#endif /* __X_NUCLEO_IKC01A1_CHARGER_H */
