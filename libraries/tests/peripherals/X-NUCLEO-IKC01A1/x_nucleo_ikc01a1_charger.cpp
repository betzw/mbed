#include "mbed.h"
#include "x_nucleo_ikc01a1_charger.h"

charger_conditions_t L6924D::GetState(void) {
	if(!st1 && st2)
		return CHARGER_CHARGE_IN_PROGRESS;
	
	if(st1 && !st2)
		return CHARGER_CHARGE_DONE;
	
	if(st1 && st2)
		return CHARGER_IN_STAND_BY;
	
	return CHARGER_INVALID_STATE;
}
