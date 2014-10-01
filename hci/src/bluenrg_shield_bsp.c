/**
******************************************************************************
* File Name          : bluenrg_shield_bsp.c
* Date               : 01/10/2014
* Description        : This file provides code for the BlueNRG Shield driver
*                      based on mbed HAL.
******************************************************************************
*
* COPYRIGHT(c) 2014 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/

#include "hci.h"
#include "spi_api.h"
#include "gpio_irq_api.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "bluenrg_shield_bsp.h"

spi_t __spi;
gpio_irq_t irq_exti;
gpio_t gpio_pin_A0, gpio_pin_CS, gpio_pin_MOSI, gpio_pin_MISO, gpio_pin_SCLK, gpio_pin_RESET;
void EXTI_irq_handler(uint32_t id, gpio_irq_event event);
	
/** @addtogroup BlueNRG_Shield
 *  @{
 */

/** @defgroup BlueNRG_Shield_Driver
 *  @brief BlueNRG Shield driver based on mbed HAL
 *  @{
 */
 
 
/**
 * @brief  This function is used for low level initialization of the SPI 
 *         communication with the BlueNRG Shield.
 * @param  hspi: handle of the STM32Cube HAL SPI interface
 * @retval None
 */
 #if 0
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	int ret;
  if(hspi->Instance==BNRG_SPI_INSTANCE)
  {        
    /* Reset */    
		gpio_init(&gpio_pin_RESET, PA_8);
		gpio_mode(&gpio_pin_RESET, PullNone);
		gpio_dir(&gpio_pin_RESET, PIN_OUTPUT);
    gpio_write(&gpio_pin_RESET, 1);
		
    /* SCLK - PA_5 - Not needed to configure if correct PinName is given to spi_init, in this case PB_3 for L0*/    
		/*gpio_init(&gpio_pin_SCLK, PB_3); //PA_5 is not USED????!!!! Since configuring PA_5 does not work!
		gpio_mode(&gpio_pin_SCLK, PullUp);
		//gpio_dir(&gpio_pin_SCLK, PIN_INPUT); //just 2 options of PIN_INPUT and PIN_OUTPUT does not suffice to configure Pin.
		pin_function(PB_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0));*/		
		
    /* NSS/CSN/CS - PA_1*/		
		gpio_init(&gpio_pin_CS, PA_1);
		gpio_mode(&gpio_pin_CS, PullNone);
		gpio_dir(&gpio_pin_CS, PIN_OUTPUT);
    gpio_write(&gpio_pin_CS, 1);
		
  }
}
#endif
/*
*	mbed EXTI IRQ Handler
*
*/
void EXTI_irq_handler(uint32_t id, gpio_irq_event event)
{
	tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  
	//Check id of the IRQ
  if(id == (uint32_t)BNRG_SPI_INSTANCE) {
    
		while (gpio_read(&gpio_pin_A0) == 1) {
      if (list_is_empty (&hciReadPktPool) == FALSE){
        /* enqueueing a packet for read */
        list_remove_head (&hciReadPktPool, (tListNode **)&hciReadPacket);
        data_len = BlueNRG_SPI_Read_All(hciReadPacket->dataBuff, HCI_PACKET_SIZE);
        
        if(data_len > 0){
          /* Packet will be inserted to the correct queue */
          HCI_Input(hciReadPacket);
        } else {
          /* Insert the packet back into the pool */
          list_insert_head(&hciReadPktPool, (tListNode *)hciReadPacket);
        }
        
      } else{
        /* TODO: HCI Read Packet Pool is empty, wait for a free packet */
      }
      
    }
  }
}

/**
* @brief  This function is used to initialize the SPI communication with
*         the BlueNRG Shield. All params should come from the User
* @param  SPI_MOSI : PA_7 (Nucleo), D11 (Generic Arduino Pin)
* @param  SPI_MISO : PA_6, D12
* @param  SPI_SCLK : PB_3, D3
* @param  SPI_CS : PA_1, A1
* @param  EXTI_IRQ : PA_0, A0
* @param  BlueNRG_RESET : PA_8, D7
* @retval None
*/
void BNRG_SPI_Init(void)
{  
  int ret;
	spi_init(&__spi, D11, D12, D3, NC);
	//spi_format(&__spi, 8, 0, 0);
  	//spi_frequency(&__spi, 1000000);
	
	/*Init IRQ for EXTI Interrupt*/	
	//gpio_init(&gpio_pin_A0, A0);//PA_0 in Nucleo
	ret = gpio_irq_init(&irq_exti, A0, EXTI_irq_handler,(uint32_t)BNRG_SPI_INSTANCE);
	gpio_irq_set(&irq_exti, IRQ_RISE, 1);//Set mode to IRQ_RISE
	gpio_init_in(&gpio_pin_A0, A0);//PA_0 in Nucleo//Configure the GPIO Pin as Input pin and PullDefault
	//gpio_irq_enable(&irq_exti);//IRQ already enabled in IRQ init call above.
	
	/* Reset Pin Config */    
	gpio_init(&gpio_pin_RESET, D7);//PA_8 in Nucleo
	gpio_mode(&gpio_pin_RESET, PullNone);
	gpio_dir(&gpio_pin_RESET, PIN_OUTPUT);
	gpio_write(&gpio_pin_RESET, 1);
	
	/* SCLK - PA_5 - Not needed to configure if correct PinName is given to spi_init, in this case PB_3 for L0*/    
	/*gpio_init(&gpio_pin_SCLK, PB_3); //PA_5 is not USED????!!!! Since configuring PA_5 does not work!
	gpio_mode(&gpio_pin_SCLK, PullUp);
	//gpio_dir(&gpio_pin_SCLK, PIN_INPUT); //just 2 options of PIN_INPUT and PIN_OUTPUT does not suffice to configure Pin.
	pin_function(PB_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0));*/
	
	/* NSS/CSN/CS - PA_1*/		
	gpio_init(&gpio_pin_CS, A1);//PA_1 in Nucleo
	gpio_mode(&gpio_pin_CS, PullNone);
	gpio_dir(&gpio_pin_CS, PIN_OUTPUT);
	gpio_write(&gpio_pin_CS, 1);
		
}

/**
* @brief  Read from BlueNRG SPI buffer and store data into local buffer
* @param  buffer:    buffer where data from SPI are stored
* @param  buff_size: buffer size
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size)
{
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t i = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char, tmpreg;
  
  uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5];
  
  /* CS reset */
  gpio_write(&gpio_pin_CS, 0);
	
  /* Read the header */
  for (i = 0; i < 5; i++)
  { 
		tmpreg = spi_master_write(&__spi, header_master[i]);
		header_slave[i] = (uint8_t)(tmpreg);
  }    
  
  
  if (header_slave[0] == 0x02) {
    /* device is ready */
    byte_count = (header_slave[4]<<8)|header_slave[3];
    
    if (byte_count > 0) {      
      /* avoid to read more data that size of the buffer */
      if (byte_count > buff_size){
        byte_count = buff_size;
      }
      
      for (len = 0; len < byte_count; len++){
				read_char = spi_master_write(&__spi, char_ff);
        buffer[len] = read_char;
      }    
    }    
  }
  /* Release CS line */
	gpio_write(&gpio_pin_CS, 1);
  
  return len;   
}

/**
* @brief  Write data from local buffer to SPI
* @param  data1:     first data buffer to be written
* @param  data2:     second data buffer to be written
* @param  Nb_bytes1: size of first data buffer to be written
* @param  Nb_bytes2: size of second data buffer to be written
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{  
  uint32_t i;
  volatile uint8_t read_char;
  int32_t result = 0;
  volatile uint8_t tmpreg;
	
  unsigned char header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[5]  = {0xaa, 0x00, 0x00, 0x00, 0x00};
  
  Disable_SPI_IRQ();
  
  /* CS reset */
	gpio_write(&gpio_pin_CS, 0);
  
  /* Exchange header */  
  for (i = 0; i < 5; i++)
  { 
		tmpreg = spi_master_write(&__spi, header_master[i]);
		header_slave[i] = tmpreg;
  }      
  
  if (header_slave[0] == 0x02) {
    /* SPI is ready */
    if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {
      /*  Buffer is big enough */
      for (i = 0; i < Nb_bytes1; i++) {
				read_char = spi_master_write(&__spi, *(data1 + i));
      }
      for (i = 0; i < Nb_bytes2; i++) {
				read_char = spi_master_write(&__spi, *(data2 + i));
      }
    } else {
      /* Buffer is too small */
      result = -2;
    }
  } else {
    /* SPI is not ready */
    result = -1;
  }
  
  /* Release CS line */
	gpio_write(&gpio_pin_CS, 1);
  
  Enable_SPI_IRQ();
  
  return result;
}

/**
 * Writes data to a serial interface. *
 * @param  data1    1st buffer
 * @param  data2    2nd buffer
 * @param  n_bytes1 number of bytes in 1st buffer
 * @param  n_bytes2 number of bytes in 2nd buffer
 */
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1, int32_t n_bytes2)
{
  struct timer t;
  
  Timer_Set(&t, CLOCK_SECOND/10);
  
  while(1){
    if(BlueNRG_SPI_Write((uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
    if(Timer_Expired(&t)){
      break;
    }
  }
}

/**
 * @brief  Disable SPI IRQ
 * @param  None
 * @retval None
 */
void Disable_SPI_IRQ(void)
{  
	gpio_irq_disable(&irq_exti);
}

/**
 * @brief  Enable SPI IRQ
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{  
	gpio_irq_enable(&irq_exti);
}

/**
 * @brief  Clear Pending SPI IRQ
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
  //Not Used
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{
	//Not Used
}

/**
 * @brief  Reset the BlueNRG
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{    
	gpio_write(&gpio_pin_RESET, 0);
  HAL_Delay(5);
	gpio_write(&gpio_pin_RESET, 1);
  HAL_Delay(5);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/