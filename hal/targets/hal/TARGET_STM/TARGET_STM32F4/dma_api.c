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
