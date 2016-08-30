/**
******************************************************************************
* @file dma_map.h
* @brief DMA hw module register map
* @internal
* @author ON Semiconductor
* $Rev: 3415 $
* $Date: 2015-06-05 13:29:52 +0530 (Fri, 05 Jun 2015) $
******************************************************************************
* @copyright (c) 2012 ON Semiconductor. All rights reserved.
* ON Semiconductor is supplying this software for use with ON Semiconductor
* processor based microcontrollers only.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup dma
*
* @details
*/

#ifndef DMA_MAP_H_
#define DMA_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** DMA control HW registers structure overlay */
#ifdef REVB
typedef struct {
    __IO uint32_t CONTROL;				/**< Write 1 to enable DMA, write 0 to disable */
    __IO uint32_t SOURCE;				/**< Address of source, read to get the number of bytes written */
    __IO uint32_t DESTINATION;			/**< Address of destination, read to get the number of bytes written  */
    __IO uint32_t SIZE;					/**< Lenght of the entire transfer */
    __IO uint32_t STATUS;				/**< To be debined */
    __IO uint32_t INT_ENABLE;			/**< Enable interrupt source by writing 1. Bit 0: DMA done, Bit 1: Source Error, Bit 2: Destination Error */
    __IO uint32_t INT_CLEAR_ENABLE;		/**< Clear Interrupt source by writing 1. Bit 0: DMA done, Bit 1: Source Error, Bit 2: Destination Error */
    __I  uint32_t INT_STATUS;			/**< Current interrupt status. Bit 0: DMA done, Bit 1: Source Error, Bit 2: Destination Error */
} DmaReg_t, *DmaReg_pt;
#endif /* REVB */
#ifdef REVD
typedef struct {
    union {
        struct {
            __IO uint32_t ENABLE:1;		/**< DMA enable:  1 to enable; 0 to disable */
            __IO uint32_t MODE	:2;		/**< DMA mode: 00 � Memory to memory; 01 � Memory to peripheral; 10 � Peripheral to memory; 11 � Peripheral to peripheral */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;							/**< Control register */
    __IO uint32_t SOURCE;				/**< Address of source, read to get the number of bytes written */
    __IO uint32_t DESTINATION;			/**< Address of destination, read to get the number of bytes written  */
    __IO uint32_t SIZE;					/**< Lenght of the entire transfer */
    union {
        struct {
            __I uint32_t COMPLETED:1;			/**< Done: 0 � Not complete, 1 � Complete */
            __I uint32_t SOURCE_ERROR:1;		/**< Source Error: 0 � No Error, 1 � Error */
            __I uint32_t DESTINATION_ERROR:1;	/**< Destination Error: 0 � No Error, 1 � Source Error */
        } BITS;
        __I uint32_t WORD;
    } STATUS;									/**< Status register */
    union {
        struct {
            __IO uint32_t COMPLETED:1;			/**< A write of �1� enables the interrupt generated by a DMA transfer complete */
            __IO uint32_t SOURCE_ERROR:1;		/**< A write of �1� enables the interrupt generated by an error on the source side of the DMA transfer */
            __IO uint32_t DESTINATION_ERROR:1;	/**< A write of �1� enables the interrupt generated by an error on the destination side of the DMA transfer */
        } BITS;
        __IO uint32_t WORD;
    } INT_ENABLE;								/**< Interrupt enable */
    union {
        struct {
            __IO uint32_t COMPLETED:1;			/**< A write clears the interrupt generated by a DMA transfer complete */
            __IO uint32_t SOURCE_ERROR:1;		/**< A write clears the interrupt generated by an error on the source side of the DMA transfer */
            __IO uint32_t DESTINATION_ERROR:1;	/**< A write clears the interrupt generated by an error on the destination side of the DMA transfer */
        } BITS;
        __IO uint32_t WORD;
    } INT_CLEAR;								/**< Interrupt clear */
    union {
        struct {
            __I uint32_t COMPLETED:1;			/**< Transfer complete interrupt */
            __I uint32_t SOURCE_ERROR:1;		/**< Source error interrupt */
            __I uint32_t DESTINATION_ERROR:1;	/**< Destination error interrupt */
        } BITS;
        __I uint32_t WORD;
    } INT_STATUS;								/**< Interrupt status */
} DmaReg_t, *DmaReg_pt;
#endif /* REVD */
#endif /* DMA_MAP_H_ */
