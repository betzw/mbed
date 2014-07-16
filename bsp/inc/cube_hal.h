#ifndef _CUBE_HAL_H_
#define _CUBE_HAL_H_
/* Includes ------------------------------------------------------------------*/
//#ifdef USE_STM32F4XX_NUCLEO
  //#include "stm32f4xx_hal.h"
  //#include "stm32f4xx_nucleo.h"
//#else
  //#ifdef USE_STM32L0XX_NUCLEO
    #include "stm32l0xx_hal.h"
    #include "stm32l0xx_nucleo.h"
  //#endif
//#endif

/* Exported functions --------------------------------------------------------*/
void SystemClock_Config(void);

#endif //_CUBE_HAL_H_