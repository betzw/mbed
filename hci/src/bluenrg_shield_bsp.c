/**
******************************************************************************
* File Name          : bluenrg_shield_bsp.c
* Date               : 16/05/2014
* Description        : This file provides code for the BlueNRG Shield driver
*                      based on STM32Cube HAL for STM32 Nucleo boards.
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
#include "cube_hal.h"
#include "hci.h"

/*//#ifdef USE_STM32F4XX_NUCLEO
  //#include "stm32f4xx_bluenrg_shield_bsp.h"
//#else
  //#ifdef USE_STM32L0XX_NUCLEO
    //#include "stm32l0xx_bluenrg_shield_bsp.h"
  //#endif
//#endif*/


/** @addtogroup BlueNRG_Shield
 *  @{
 */

/** @defgroup BlueNRG_Shield_Driver
 *  @brief BlueNRG Shield driver based on STM32Cube HAL for STM32 Nucleo boards.
 *  @{
 */


/* SPI handler declared in "main.c" file */
extern SPI_HandleTypeDef SpiHandle;


/**
 * @brief  This function is used for low level initialization of the SPI 
 *         communication with the BlueNRG Shield.
 * @param  hspi: handle of the STM32Cube HAL SPI interface
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==BNRG_SPI_INSTANCE)
  {
    /* Enable peripherals clock */
    
    /* Enable GPIO Ports Clock */  
    BNRG_SPI_RESET_CLK_ENABLE();
    BNRG_SPI_SCLK_CLK_ENABLE();
    BNRG_SPI_MISO_CLK_ENABLE();
    BNRG_SPI_MOSI_CLK_ENABLE();
    BNRG_SPI_CS_CLK_ENABLE();
    BNRG_SPI_IRQ_CLK_ENABLE();
    
    /* Enable SPI clock */
    BNRG_SPI_CLK_ENABLE();
    
    /* Reset */
    GPIO_InitStruct.Pin = BNRG_SPI_RESET_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_RESET_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_RESET_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_RESET_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_RESET_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_RESET_PORT, &GPIO_InitStruct);	
    HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);	/*Added to avoid spurious interrupt from the BlueNRG */
    
    /* SCLK */
    GPIO_InitStruct.Pin = BNRG_SPI_SCLK_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_SCLK_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_SCLK_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_SCLK_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_SCLK_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_SCLK_PORT, &GPIO_InitStruct); 
    
    /* MISO */
    GPIO_InitStruct.Pin = BNRG_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MISO_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MISO_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MISO_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MISO_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MISO_PORT, &GPIO_InitStruct);
    
    /* MOSI */
    GPIO_InitStruct.Pin = BNRG_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MOSI_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MOSI_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MOSI_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MOSI_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MOSI_PORT, &GPIO_InitStruct);
    
    /* NSS/CSN/CS */
    GPIO_InitStruct.Pin = BNRG_SPI_CS_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_CS_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_CS_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_CS_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_CS_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_CS_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
    
    /* IRQ -- INPUT */
    GPIO_InitStruct.Pin = BNRG_SPI_IRQ_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_IRQ_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_IRQ_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_IRQ_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_IRQ_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStruct);
    
    /* Configure the NVIC for SPI */  
    HAL_NVIC_SetPriority(BNRG_SPI_EXTI_IRQn, 4, 0);    
    HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
  }
}

/**
 * @brief EXTI line detection callback.
 * @param GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  /* 
   * No need to call Clear_SPI_EXTI_Flag() here as
   * HAL_GPIO_EXTI_IRQHandler() already does it
   */
    
  if(GPIO_Pin == BNRG_SPI_EXTI_PIN) {
    
    while (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET) {
      if (list_is_empty (&hciReadPktPool) == FALSE){
        /* enqueueing a packet for read */
        list_remove_head (&hciReadPktPool, (tListNode **)&hciReadPacket);
        data_len = BlueNRG_SPI_Read_All(&SpiHandle, hciReadPacket->dataBuff, HCI_PACKET_SIZE);
        
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
      
      Clear_SPI_EXTI_Flag();
    }
  }
}


/**
* @brief  This function is used to initialize the SPI communication with
*         the BlueNRG Shield.
* @param  None
* @retval None
*/
void BNRG_SPI_Init(void)
{
  SpiHandle.Instance = BNRG_SPI_INSTANCE;
  SpiHandle.Init.Mode = BNRG_SPI_MODE;
  SpiHandle.Init.Direction = BNRG_SPI_DIRECTION;
  SpiHandle.Init.DataSize = BNRG_SPI_DATASIZE;
  SpiHandle.Init.CLKPolarity = BNRG_SPI_CLKPOLARITY;
  SpiHandle.Init.CLKPhase = BNRG_SPI_CLKPHASE;
  SpiHandle.Init.NSS = BNRG_SPI_NSS;
  SpiHandle.Init.FirstBit = BNRG_SPI_FIRSTBIT;
  SpiHandle.Init.TIMode = BNRG_SPI_TIMODE;
  SpiHandle.Init.CRCPolynomial = BNRG_SPI_CRCPOLYNOMIAL;
  SpiHandle.Init.BaudRatePrescaler = BNRG_SPI_BAUDRATEPRESCALER;
  SpiHandle.Init.CRCCalculation = BNRG_SPI_CRCCALCULATION;
  
  HAL_SPI_Init(&SpiHandle);
}

/**
* @brief  Read from BlueNRG SPI buffer and store data into local buffer
* @param  hspi:      handle of the STM32Cube HAL SPI interface
* @param  buffer:    buffer where data from SPI are stored
* @param  buff_size: buffer size
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Read_All(SPI_HandleTypeDef *hspi, uint8_t *buffer, uint8_t buff_size)
{
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t i = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char;
  
  uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5];
  
  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
  
  /* Read the header */
  for (i = 0; i < 5; i++)
  { 
    HAL_SPI_TransmitReceive(hspi, &header_master[i], &header_slave[i], 1, 15);
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
        HAL_SPI_TransmitReceive(hspi, &char_ff, (uint8_t*)&read_char, 1, 15);
        buffer[len] = read_char;
      }    
    }    
  }
  /* Release CS line */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  
  return len;   
}

/**
* @brief  Write data from local buffer to SPI
* @param  hspi:      handle of the STM32Cube HAL SPI interface
* @param  data1:     first data buffer to be written
* @param  data2:     second data buffer to be written
* @param  Nb_bytes1: size of first data buffer to be written
* @param  Nb_bytes2: size of second data buffer to be written
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Write(SPI_HandleTypeDef *hspi, uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{  
  uint32_t i;
  uint8_t read_char;
  int32_t result = 0;
  
  unsigned char header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[5]  = {0xaa, 0x00, 0x00, 0x00, 0x00};
  
  Disable_SPI_IRQ();
  
  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
  
  /* Exchange header */
  
  
  for (i = 0; i < 5; i++)
  { 
    HAL_SPI_TransmitReceive(hspi, &header_master[i], &header_slave[i], 1, 15);
  }    
  
  
  if (header_slave[0] == 0x02) {
    /* SPI is ready */
    if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {
      /*  Buffer is big enough */
      for (i = 0; i < Nb_bytes1; i++) {
        HAL_SPI_TransmitReceive(hspi, (data1 + i), &read_char, 1, 15);
      }
      for (i = 0; i < Nb_bytes2; i++) {
        HAL_SPI_TransmitReceive(hspi, (data2 + i), &read_char, 1, 15);
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
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  
  Enable_SPI_IRQ();
  
  return result;
}

/**
 * Writes data to a serial interface.
 *
 * @param  data1    1st buffer
 * @param  data2    2nd buffer
 * @param  n_bytes1 number of bytes in 1st buffer
 * @param  n_bytes2 number of bytes in 2nd buffer
 */
void Hal_Write_Serial(const void* data1, const void* data2, tHalInt32 n_bytes1, tHalInt32 n_bytes2)
{
  struct timer t;
  
  Timer_Set(&t, CLOCK_SECOND/10);
  
  while(1){
    if(BlueNRG_SPI_Write(&SpiHandle, (uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
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
  HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Enable SPI IRQ
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{  
  HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);  
}

/**
 * @brief  Clear Pending SPI IRQ
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{
  __HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);
}

/**
 * @brief  Reset the BlueNRG
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{    
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/