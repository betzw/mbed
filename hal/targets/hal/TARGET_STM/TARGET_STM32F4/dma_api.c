/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 *******************************************************************************
 */
#include "dma_api.h"
#include "dma_caps.h"
#include "device.h"
#include "stm32f401xe.h"

static struct dma_stream_s dma_map[NUM_OF_DEVICES][NUM_OF_DIRECTIONS];
static unsigned int dma1_ref_counter = 0;

void dma_init(void) {
	static int inited = 0;

	if(inited) return;

	inited = 1;

	// SPI2 - TX
	dma_map[DMA_SPI2][DMA_TX].dma_stream = DMA1_Stream4;
	dma_map[DMA_SPI2][DMA_TX].dma_stream_irq = DMA1_Stream4_IRQn;
	dma_map[DMA_SPI2][DMA_TX].channel_nr = DMA_CHANNEL_0;
	dma_map[DMA_SPI2][DMA_TX].channel_nr_fd = DMA_CHANNEL_2;
	dma_map[DMA_SPI2][DMA_TX].busy = 0;

	// SPI2 - RX
	dma_map[DMA_SPI2][DMA_RX].dma_stream = DMA1_Stream3;
	dma_map[DMA_SPI2][DMA_RX].dma_stream_irq = DMA1_Stream3_IRQn;
	dma_map[DMA_SPI2][DMA_RX].channel_nr = DMA_CHANNEL_0;
	dma_map[DMA_SPI2][DMA_RX].channel_nr_fd = DMA_CHANNEL_3;
	dma_map[DMA_SPI2][DMA_RX].busy = 0;

	// SPI3 - TX
	dma_map[DMA_SPI3][DMA_TX].dma_stream = DMA1_Stream5;
	dma_map[DMA_SPI3][DMA_TX].dma_stream_irq = DMA1_Stream5_IRQn;
	dma_map[DMA_SPI3][DMA_TX].channel_nr = DMA_CHANNEL_0;
	dma_map[DMA_SPI3][DMA_TX].channel_nr_fd = DMA_CHANNEL_2;
	dma_map[DMA_SPI3][DMA_TX].busy = 0;

	// SPI3 - RX
	dma_map[DMA_SPI3][DMA_RX].dma_stream = DMA1_Stream0;
	dma_map[DMA_SPI3][DMA_RX].dma_stream_irq = DMA1_Stream0_IRQn;
	dma_map[DMA_SPI3][DMA_RX].channel_nr = DMA_CHANNEL_0;
	dma_map[DMA_SPI3][DMA_RX].channel_nr_fd = DMA_CHANNEL_3;
	dma_map[DMA_SPI3][DMA_RX].busy = 0;
}

int dma_channel_allocate(uint32_t capabilities) {
	int device    = GET_DMA_DEVICE(capabilities);
	int direction = GET_DMA_TXRX(capabilities);

	struct dma_stream_s *channel = &dma_map[device][direction];

	if(channel->busy)
		return DMA_ERROR_OUT_OF_CHANNELS;
	else {
		if(!(dma1_ref_counter++)) {
			  __DMA1_CLK_ENABLE();
		}
		channel->busy = 1;
		return (void*)channel;
	}
}

int dma_channel_free(int channelid) {
	struct dma_stream_s *channel = (struct dma_stream_s*)channelid;

	MBED_ASSERT((channel->busy == 1) && (dma1_ref_counter > 0));
	channel->busy = 0;

	if(!(--dma1_ref_counter)) {
		  __DMA1_FORCE_RESET();
		  __DMA1_RELEASE_RESET();
		  __DMA1_CLK_DISABLE();
	}

	return;
}
