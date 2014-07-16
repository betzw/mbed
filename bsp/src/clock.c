#include "cube_hal.h"
#include "clock.h"


const tHalUint32 CLOCK_SECOND = 1000;

/*---------------------------------------------------------------------------*/

void Clock_Init(void)
{
  // FIXME: as long as Cube HAL is initialized this is OK
  // Cube HAL default is one clock each 1 ms
}

/*---------------------------------------------------------------------------*/

tClockTime Clock_Time(void)
{
  return HAL_GetTick();
}

/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 1 ms.
 *
 */
void Clock_Wait(int i)
{
  HAL_Delay(i);
}
/*---------------------------------------------------------------------------*/

