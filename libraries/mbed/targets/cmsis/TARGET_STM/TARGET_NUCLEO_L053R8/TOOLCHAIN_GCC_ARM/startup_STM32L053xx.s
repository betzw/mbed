/******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
 * File Name          : startup_stm32l053xx.s
 * Author             : AST / EST
 * Version            : V0.0.1
 * Date               : 10-October-2014
 * Description        : STM32l053xx Devices vector table for ARM GCC toolchain.
 *                      This module performs:
 *                      - Set the initial SP
 *                      - Set the initial PC == Reset_Handler
 *                      - Set the vector table entries with the exceptions ISR address
 *                      - Branches to __main in the C library (which eventually
 *                        calls main()).
 *                      After Reset the Cortex-M0+ processor is in Thread mode,
 *                      priority is Privileged, and the Stack is set to Main.
 * <<< Use Configuration Wizard in Context Menu >>>   
 *******************************************************************************
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
 *******************************************************************************/
/* File: startup_STM32L053xx.S
 * Purpose: startup file for Cortex-M4 devices. Should use with
 *   GCC for ARM Embedded Processors
 * Version: V0.1
 * Date: 30 October 2014
 *
 * Copyright (c) 2011, 2012, ARM Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
    .syntax unified
    .arch armv6-m

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x400
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x80
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External interrupts */
	.long     WWDG_IRQHandler                /* Window Watchdog */
	.long     PVD_IRQHandler                 /* PVD through EXTI Line detect */
	.long     RTC_IRQHandler                 /* RTC through EXTI Line */
	.long     FLASH_IRQHandler               /* FLASH */
	.long     RCC_CRS_IRQHandler             /* RCC and CRS */
	.long     EXTI0_1_IRQHandler             /* EXTI Line 0 and 1 */
	.long     EXTI2_3_IRQHandler             /* EXTI Line 2 and 3 */
	.long     EXTI4_15_IRQHandler            /* EXTI Line 4 to 15 */
	.long     TSC_IRQHandler                 /* TSC */
	.long     DMA1_Channel1_IRQHandler       /* DMA1 Channel 1 */
	.long     DMA1_Channel2_3_IRQHandler     /* DMA1 Channel 2 and Channel 3 */
	.long     DMA1_Channel4_5_6_7_IRQHandler /* DMA1 Channel 4, Channel 5, Channel 6 and Channel 7 */
	.long     ADC1_COMP_IRQHandler           /* ADC1, COMP1 and COMP2 */
	.long     LPTIM1_IRQHandler              /* LPTIM1 */
	.long     0                              /* Reserved */
	.long     TIM2_IRQHandler                /* TIM2 */
	.long     0                              /* Reserved */
	.long     TIM6_DAC_IRQHandler            /* TIM6 and DAC */
	.long     0                              /* Reserved */
	.long     0                              /* Reserved */
	.long     TIM21_IRQHandler               /* TIM21 */
	.long     0                              /* Reserved */
	.long     TIM22_IRQHandler               /* TIM22 */
	.long     I2C1_IRQHandler                /* I2C1 */
	.long     I2C2_IRQHandler                /* I2C2 */
	.long     SPI1_IRQHandler                /* SPI1 */
	.long     SPI2_IRQHandler                /* SPI2 */
	.long     USART1_IRQHandler              /* USART1 */
	.long     USART2_IRQHandler              /* USART2 */
	.long     RNG_LPUART1_IRQHandler         /* RNG and LPUART1 */
	.long     LCD_IRQHandler                 /* LCD */
	.long     USB_IRQHandler                 /* USB */

    .size    __isr_vector, . - __isr_vector

    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble    .Lflash_to_ram_loop_end

    movs    r4, 0
.Lflash_to_ram_loop:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds   r4, 4
    cmp    r4, r3
    blt    .Lflash_to_ram_loop
.Lflash_to_ram_loop_end:

    ldr   r0, =SystemInit
    blx   r0
    ldr   r0, =_start
    bx    r0
    .pool
    .size Reset_Handler, . - Reset_Handler

    .text
/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    def_default_handler    Default_Handler

    .macro    def_irq_default_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

        def_irq_default_handler  WWDG_IRQHandler
        def_irq_default_handler  PVD_IRQHandler
        def_irq_default_handler  RTC_IRQHandler
        def_irq_default_handler  FLASH_IRQHandler
        def_irq_default_handler  RCC_CRS_IRQHandler
        def_irq_default_handler  EXTI0_1_IRQHandler
        def_irq_default_handler  EXTI2_3_IRQHandler
        def_irq_default_handler  EXTI4_15_IRQHandler
        def_irq_default_handler  TSC_IRQHandler
        def_irq_default_handler  DMA1_Channel1_IRQHandler
        def_irq_default_handler  DMA1_Channel2_3_IRQHandler
        def_irq_default_handler  DMA1_Channel4_5_6_7_IRQHandler
        def_irq_default_handler  ADC1_COMP_IRQHandler
        def_irq_default_handler  LPTIM1_IRQHandler
        def_irq_default_handler  TIM2_IRQHandler
        def_irq_default_handler  TIM6_DAC_IRQHandler
        def_irq_default_handler  TIM21_IRQHandler
        def_irq_default_handler  TIM22_IRQHandler
        def_irq_default_handler  I2C1_IRQHandler
        def_irq_default_handler  I2C2_IRQHandler
        def_irq_default_handler  SPI1_IRQHandler
        def_irq_default_handler  SPI2_IRQHandler
        def_irq_default_handler  USART1_IRQHandler
        def_irq_default_handler  USART2_IRQHandler
        def_irq_default_handler  RNG_LPUART1_IRQHandler
        def_irq_default_handler  LCD_IRQHandler
        def_irq_default_handler  USB_IRQHandler

    .end
