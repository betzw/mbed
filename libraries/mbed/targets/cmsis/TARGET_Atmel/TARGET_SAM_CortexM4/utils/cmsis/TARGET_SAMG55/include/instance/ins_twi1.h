/**
 * \file
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAMG55_TWI1_INSTANCE_
#define _SAMG55_TWI1_INSTANCE_

/* ========== Register definition for TWI1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI1_CR                     (0x40020600U) /**< \brief (TWI1) TWI Control Register */
#define REG_TWI1_MMR                    (0x40020604U) /**< \brief (TWI1) TWI Master Mode Register */
#define REG_TWI1_SMR                    (0x40020608U) /**< \brief (TWI1) TWI Slave Mode Register */
#define REG_TWI1_IADR                   (0x4002060CU) /**< \brief (TWI1) TWI Internal Address Register */
#define REG_TWI1_CWGR                   (0x40020610U) /**< \brief (TWI1) TWI Clock Waveform Generator Register */
#define REG_TWI1_SR                     (0x40020620U) /**< \brief (TWI1) TWI Status Register */
#define REG_TWI1_IER                    (0x40020624U) /**< \brief (TWI1) TWI Interrupt Enable Register */
#define REG_TWI1_IDR                    (0x40020628U) /**< \brief (TWI1) TWI Interrupt Disable Register */
#define REG_TWI1_IMR                    (0x4002062CU) /**< \brief (TWI1) TWI Interrupt Mask Register */
#define REG_TWI1_RHR                    (0x40020630U) /**< \brief (TWI1) TWI Receive Holding Register */
#define REG_TWI1_THR                    (0x40020634U) /**< \brief (TWI1) TWI Transmit Holding Register */
#define REG_TWI1_SMBTR                  (0x40020638U) /**< \brief (TWI1) TWI SMBus Timing Register */
#define REG_TWI1_ACR                    (0x40020640U) /**< \brief (TWI1) TWI Alternative Command Register */
#define REG_TWI1_FILTR                  (0x40020644U) /**< \brief (TWI1) TWI Filter Register */
#define REG_TWI1_SWMR                   (0x4002064CU) /**< \brief (TWI1) TWI SleepWalking Matching Register */
#define REG_TWI1_WPMR                   (0x400206E4U) /**< \brief (TWI1) TWI Write Protection Mode Register */
#define REG_TWI1_WPSR                   (0x400206E8U) /**< \brief (TWI1) TWI Write Protection Status Register */
#define REG_TWI1_RPR                    (0x40020700U) /**< \brief (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR                    (0x40020704U) /**< \brief (TWI1) Receive Counter Register */
#define REG_TWI1_TPR                    (0x40020708U) /**< \brief (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR                    (0x4002070CU) /**< \brief (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR                   (0x40020710U) /**< \brief (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR                   (0x40020714U) /**< \brief (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR                   (0x40020718U) /**< \brief (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR                   (0x4002071CU) /**< \brief (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR                   (0x40020720U) /**< \brief (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR                   (0x40020724U) /**< \brief (TWI1) Transfer Status Register */
#else
#define REG_TWI1_CR    (*(__O  uint32_t*)0x40020600U) /**< \brief (TWI1) TWI Control Register */
#define REG_TWI1_MMR   (*(__IO uint32_t*)0x40020604U) /**< \brief (TWI1) TWI Master Mode Register */
#define REG_TWI1_SMR   (*(__IO uint32_t*)0x40020608U) /**< \brief (TWI1) TWI Slave Mode Register */
#define REG_TWI1_IADR  (*(__IO uint32_t*)0x4002060CU) /**< \brief (TWI1) TWI Internal Address Register */
#define REG_TWI1_CWGR  (*(__IO uint32_t*)0x40020610U) /**< \brief (TWI1) TWI Clock Waveform Generator Register */
#define REG_TWI1_SR    (*(__I  uint32_t*)0x40020620U) /**< \brief (TWI1) TWI Status Register */
#define REG_TWI1_IER   (*(__O  uint32_t*)0x40020624U) /**< \brief (TWI1) TWI Interrupt Enable Register */
#define REG_TWI1_IDR   (*(__O  uint32_t*)0x40020628U) /**< \brief (TWI1) TWI Interrupt Disable Register */
#define REG_TWI1_IMR   (*(__I  uint32_t*)0x4002062CU) /**< \brief (TWI1) TWI Interrupt Mask Register */
#define REG_TWI1_RHR   (*(__I  uint32_t*)0x40020630U) /**< \brief (TWI1) TWI Receive Holding Register */
#define REG_TWI1_THR   (*(__O  uint32_t*)0x40020634U) /**< \brief (TWI1) TWI Transmit Holding Register */
#define REG_TWI1_SMBTR (*(__IO uint32_t*)0x40020638U) /**< \brief (TWI1) TWI SMBus Timing Register */
#define REG_TWI1_ACR   (*(__IO uint32_t*)0x40020640U) /**< \brief (TWI1) TWI Alternative Command Register */
#define REG_TWI1_FILTR (*(__IO uint32_t*)0x40020644U) /**< \brief (TWI1) TWI Filter Register */
#define REG_TWI1_SWMR  (*(__IO uint32_t*)0x4002064CU) /**< \brief (TWI1) TWI SleepWalking Matching Register */
#define REG_TWI1_WPMR  (*(__IO uint32_t*)0x400206E4U) /**< \brief (TWI1) TWI Write Protection Mode Register */
#define REG_TWI1_WPSR  (*(__I  uint32_t*)0x400206E8U) /**< \brief (TWI1) TWI Write Protection Status Register */
#define REG_TWI1_RPR   (*(__IO uint32_t*)0x40020700U) /**< \brief (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR   (*(__IO uint32_t*)0x40020704U) /**< \brief (TWI1) Receive Counter Register */
#define REG_TWI1_TPR   (*(__IO uint32_t*)0x40020708U) /**< \brief (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR   (*(__IO uint32_t*)0x4002070CU) /**< \brief (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR  (*(__IO uint32_t*)0x40020710U) /**< \brief (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR  (*(__IO uint32_t*)0x40020714U) /**< \brief (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR  (*(__IO uint32_t*)0x40020718U) /**< \brief (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR  (*(__IO uint32_t*)0x4002071CU) /**< \brief (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR  (*(__O  uint32_t*)0x40020720U) /**< \brief (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR  (*(__I  uint32_t*)0x40020724U) /**< \brief (TWI1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI1_INSTANCE_ */
