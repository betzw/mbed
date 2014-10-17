#include "mbed.h"
#include "x_nucleo_ikc01a1.h"

X_NUCLEO_IKC01A1* X_NUCLEO_IKC01A1::_instance = NULL;
X_NUCLEO_IKC01A1* X_NUCLEO_IKC01A1::Instance() {
	if(_instance == NULL) {
		_instance = new X_NUCLEO_IKC01A1();
	}
	return _instance;
}
  
