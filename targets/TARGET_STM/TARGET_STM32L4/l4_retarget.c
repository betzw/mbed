/**
  ******************************************************************************
  * @file    l4_retarget.c
 * @author   MCD Application Team
  * @brief   CMSIS Cortex-M4 Core Peripheral Access Layer Source File for STM32L475xG
  ******************************************************************************  
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
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
#if (defined(TWO_RAM_REGIONS) && defined(__GNUC__) && !defined(__CC_ARM))
#include <errno.h>
#include "stm32l4xx.h"

extern uint32_t __end__;

/* Heap limits - only used if set */
extern unsigned char *mbed_heap_start;
extern uint32_t mbed_heap_size;

/**
 * The default implementation of _sbrk() (in platform/mbed_retarget.cpp) for GCC_ARM requires one-region model (heap and 
 * stack share one region), which doesn't fit two-region model (heap and stack are two distinct regions), for example, 
 * STM32L475xG locates heap on SRAM1 and stack on SRAM2. 
 * Define __wrap__sbrk() to override the default _sbrk(). It is expected to get called through gcc 
 * hooking mechanism ('-Wl,--wrap,_sbrk') or in _sbrk().
 */
void *__wrap__sbrk(int incr)
{
    static unsigned char* heap = (unsigned char*)&__end__;
    unsigned char*        prev_heap = heap;
    unsigned char*        new_heap = heap + incr;

#if !MBED_CONF_RTOS_PRESENT
    if (new_heap >= (unsigned char*)__get_MSP()) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }
#endif

    // Additional heap checking if set
    if (mbed_heap_size && (new_heap >= mbed_heap_start + mbed_heap_size)) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}
#endif /* GCC_ARM toolchain && TWO_RAM_REGIONS*/

