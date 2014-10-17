/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __X_NUCLEO_IKC01A1_H
#define __X_NUCLEO_IKC01A1_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_ikc01a1_charger.h"

/* Classes -------------------------------------------------------------------*/
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
