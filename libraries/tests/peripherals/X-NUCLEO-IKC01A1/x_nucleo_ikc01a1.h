/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __X_NUCLEO_IKC01A1_H
#define __X_NUCLEO_IKC01A1_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1_charger.h"

/* Classes -------------------------------------------------------------------*/
/* Class X_NUCLEO_IKC01A1 is intended to represent the battery management 
   expansion board with the same name, featuring basically three IPs:
   1) a battery charger of class L6924D
   2) a real-time clock (RTC) of class M41T62
   3) and a gas gauge (GG) of class STC3115
   It is intentionally implemented as a singleton because only one 
   X_NUCLEO_IKC01A1 at a time might be deployed in a HW component stack.
   In order to get the singleton instance you have to call class method
   X_NUCLEO_IKC01A1::Instance().
*/
class X_NUCLEO_IKC01A1 {
 public:
	static X_NUCLEO_IKC01A1* Instance();

	L6924D charger;

 protected:
        X_NUCLEO_IKC01A1() : charger() {};

 private:
	static X_NUCLEO_IKC01A1 *_instance;
};

#endif /* __X_NUCLEO_IKC01A1_H */
