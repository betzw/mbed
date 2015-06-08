/**
  ******************************************************************************
  * @file    stm32_bluenrg_ble.c
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "stm32_bluenrg_ble.h"
#include "gp_timer.h"
#include "debug.h"

#include "hci.h"
#include "spi_api.h"
#include "gpio_irq_api.h"
#include "gpio_api.h"
#include "wait_api.h"
#include "bluenrg_platform.h"

//extern volatile uint32_t ms_counter;

/** @addtogroup BSP
 *  @{
 */

/** @defgroup X-NUCLEO-IDB04A1
 *  @{
 */
 
/** @defgroup STM32_BLUENRG_BLE
 *  @{
 */

/** @defgroup STM32_BLUENRG_BLE_Private_Defines 
 * @{
 */ 

#define HEADER_SIZE 5
#define MAX_BUFFER_SIZE 255
#define TIMEOUT_DURATION 15

spi_t __spi;
gpio_irq_t irq_exti;
gpio_t gpio_pin_A0, gpio_pin_CS, gpio_pin_MOSI, gpio_pin_MISO;
gpio_t gpio_pin_SCLK, gpio_pin_RESET;


/**
 * @}
 */

/** @defgroup STM32_BLUENRG_BLE_Private_Variables
 * @{
 */

SPI_HandleTypeDef SpiHandle;

/**
 * @}
 */

/** @defgroup STM32_BLUENRG_BLE_Private_Function_Prototypes 
 *  @{
 */

/* Private function prototypes -----------------------------------------------*/
static void us150Delay(void);
void set_irq_as_output(void);
void set_irq_as_input(void);

void EXTI_irq_handler(uint32_t id, gpio_irq_event event);

/**
 * @}
 */ 

/** @defgroup STM32_BLUENRG_BLE_Exported_Functions 
 * @{
 */ 
 
/*
*	mbed EXTI IRQ Handler
*
*/
void EXTI_irq_handler(uint32_t id, gpio_irq_event event)
{
  //Check id of the IRQ
  if(id == (uint32_t)BNRG_SPI_INSTANCE) {
	  HCI_Isr();
  }
} 
 

/**
 * @brief  This function is a utility to print the log time
*          in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
//void print_csv_time(void){
//  uint32_t ms = ms_counter;
//  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
//}

#ifdef CUBE
/**
 * @brief  This function is used for low level initialization of the SPI 
 *         communication with the BlueNRG Expansion Board.
 * @param  hspi: SPI handle.
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
    HAL_NVIC_SetPriority(BNRG_SPI_EXTI_IRQn, 3, 0);    
    HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
  }
}
#endif
/**
 * @brief  Writes data to a serial interface.
 * @param  data1   :  1st buffer
 * @param  data2   :  2nd buffer
 * @param  n_bytes1: number of bytes in 1st buffer
 * @param  n_bytes2: number of bytes in 2nd buffer
 * @retval None
 */
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1,
                      int32_t n_bytes2)
{
  struct timer t;

  Timer_Set(&t, CLOCK_SECOND/10);

#ifdef PRINT_CSV_FORMAT
//  print_csv_time();
  for (int i=0; i<n_bytes1; i++) {
    PRINT_CSV(" %02x", ((uint8_t *)data1)[i]);
	 }
  for (int i=0; i<n_bytes2; i++) {
    PRINT_CSV(" %02x", ((uint8_t *)data2)[i]);
	 }
  PRINT_CSV("\n");
#endif

  while(1){
    if(BlueNRG_SPI_Write(&SpiHandle, (uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
    if(Timer_Expired(&t)){
      break;
    }
  }
}

#ifdef CUBE
/**
 * @brief  Initializes the SPI communication with the BlueNRG
 *         Expansion Board.
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
#endif
/**
* @brief  This function is used to initialize the SPI communication with
*         the BlueNRG Shield. All params should come from the User
* @param  SPI_MOSI : PA_7 (Nucleo), D11 (Generic Arduino Pin)
* @param  SPI_MISO : PA_6, D12
* @param  SPI_SCLK : PB_3, D3 (Nucleo)
* @param  SPI_CS : PA_1, A1
* @param  EXTI_IRQ : PA_0, A0
* @param  BlueNRG_RESET : PA_8, D7
* @retval None
*/
void BNRG_SPI_Init(PinName mosi, PinName miso, PinName sclk)
{
	//All pins configured in platform.h according to platform chosen
	spi_init(&__spi, PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_CLK, NC);
	
	/* Reset Pin Config */    
	gpio_init(&gpio_pin_RESET, PIN_SPI_RESET);//PA_8 in Nucleo
	gpio_mode(&gpio_pin_RESET, PullNone);
	gpio_dir(&gpio_pin_RESET, PIN_OUTPUT);
	gpio_write(&gpio_pin_RESET, 1);
	
	/* SCLK - PA_5 - Not needed to configure if correct PinName is given to spi_init, in this case PB_3 for L0*/    
	/*gpio_init(&gpio_pin_SCLK, PB_3); //PA_5 is not USED????!!!! Since configuring PA_5 does not work!
	gpio_mode(&gpio_pin_SCLK, PullUp);
	//gpio_dir(&gpio_pin_SCLK, PIN_INPUT); //just 2 options of PIN_INPUT and PIN_OUTPUT does not suffice to configure Pin.
	pin_function(PB_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0));*/
	
	/* NSS/CSN/CS - PA_1*/		
	gpio_init(&gpio_pin_CS, PIN_SPI_CS);//PA_1 in Nucleo
	gpio_mode(&gpio_pin_CS, PullNone);
	gpio_dir(&gpio_pin_CS, PIN_OUTPUT);
	gpio_write(&gpio_pin_CS, 1);
	
	/*Init IRQ for EXTI Interrupt*/	
	gpio_irq_init(&irq_exti, PIN_SPI_IRQ, EXTI_irq_handler,(uint32_t)BNRG_SPI_INSTANCE);
	gpio_irq_set(&irq_exti, IRQ_RISE, 1);//Set mode to IRQ_RISE
	//gpio_init_in(&gpio_pin_A0, PTA2);//Configure the GPIO Pin as Input pin and PullDefault//gpio_init_in() does gpio_init() anyway
	
	/* EXTI IRQ Pin Config*/		
	gpio_init(&gpio_pin_A0, PIN_SPI_IRQ);
	gpio_mode(&gpio_pin_A0, PullNone);
	gpio_dir(&gpio_pin_A0, PIN_INPUT);
	gpio_write(&gpio_pin_A0, 0);
			
}

/**
 * @brief  Resets the BlueNRG.
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{
#ifdef CUBE
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
#endif	
	gpio_write(&gpio_pin_RESET, 0);
  	wait_us(5);
	gpio_write(&gpio_pin_RESET, 1);
  	wait_us(5);
}

/**
 * @brief  Reports if the BlueNRG has data for the host micro.
 * @param  None
 * @retval 1 if data are present, 0 otherwise
 */
// FIXME: find a better way to handle this return value (bool type? TRUE and FALSE)
uint8_t BlueNRG_DataPresent(void)
{
  //if (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET)
	if	(gpio_read(&gpio_pin_A0) == 1)
      return 1;
  else  
      return 0;
} /* end BlueNRG_DataPresent() */

/**
 * @brief  Activate internal bootloader using pin.
 * @param  None
 * @retval None
 */
void BlueNRG_HW_Bootloader(void)
{
  set_irq_as_output();
  BlueNRG_RST();
  set_irq_as_input();
}

/**
 * @brief  Reads from BlueNRG SPI buffer and store data into local buffer.
 * @param  hspi     : SPI handle
 * @param  buffer   : Buffer where data from SPI are stored
 * @param  buff_size: Buffer size
 * @retval int32_t  : Number of read bytes
 */
int32_t BlueNRG_SPI_Read_All(SPI_HandleTypeDef *hspi, uint8_t *buffer,
                             uint8_t buff_size)
{
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char;
	
	uint8_t i = 0;
	volatile uint8_t tmpreg;

  uint8_t header_master[HEADER_SIZE] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[HEADER_SIZE];

  /* CS reset */
  //HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
  gpio_write(&gpio_pin_CS, 0);
	
  /* Read the header */  
  //HAL_SPI_TransmitReceive(hspi, header_master, header_slave, HEADER_SIZE, TIMEOUT_DURATION);
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
        //HAL_SPI_TransmitReceive(hspi, &char_ff, (uint8_t*)&read_char, 1, TIMEOUT_DURATION);
        read_char = spi_master_write(&__spi, char_ff);
				buffer[len] = read_char;
      }
    }    
  }
  /* Release CS line */
  //HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  gpio_write(&gpio_pin_CS, 1);
	
  // Add a small delay to give time to the BlueNRG to set the IRQ pin low
  // to avoid a useless SPI read at the end of the transaction
  for(volatile int i = 0; i < 2; i++)__NOP();
  
#ifdef PRINT_CSV_FORMAT
  if (len > 0) {
//    print_csv_time();
    for (int i=0; i<len; i++) {
      PRINT_CSV(" %02x", buffer[i]);
    }
    PRINT_CSV("\n");
  }
#endif
  
  return len;   
}

/**
 * @brief  Writes data from local buffer to SPI.
 * @param  hspi     : SPI handle
 * @param  data1    : First data buffer to be written
 * @param  data2    : Second data buffer to be written
 * @param  Nb_bytes1: Size of first data buffer to be written
 * @param  Nb_bytes2: Size of second data buffer to be written
 * @retval Number of read bytes
 */
int32_t BlueNRG_SPI_Write(SPI_HandleTypeDef *hspi, uint8_t* data1,
                          uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{  
  int32_t result = 0;
  int32_t spi_fix_enabled = 0;
	
	uint32_t i;
	volatile uint8_t read_char;
	volatile uint8_t tmpreg;
  
#ifdef ENABLE_SPI_FIX
  spi_fix_enabled = 1;
#endif //ENABLE_SPI_FIX
  
  unsigned char header_master[HEADER_SIZE] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[HEADER_SIZE]  = {0xaa, 0x00, 0x00, 0x00, 0x00};
  
  unsigned char read_char_buf[MAX_BUFFER_SIZE];

  Disable_SPI_IRQ(); 

  /*
   If the SPI_FIX is enabled the IRQ is set in Output mode, then it is pulled
   high and, after a delay of at least 112us, the CS line is asserted and the
   header transmit/receive operations are started.
   After these transmit/receive operations the IRQ is reset in input mode.
 */
  if (spi_fix_enabled) {
    set_irq_as_output();

    /* Assert CS line after at least 112us */
    us150Delay();
}

  /* CS reset */
  //HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
  gpio_write(&gpio_pin_CS, 0);

  /* Exchange header */  
  //HAL_SPI_TransmitReceive(hspi, header_master, header_slave, HEADER_SIZE, TIMEOUT_DURATION);
  for (i = 0; i < 5; i++)
  { 
		tmpreg = spi_master_write(&__spi, header_master[i]);
		header_slave[i] = tmpreg;
  } 
	
  if (spi_fix_enabled) {
    set_irq_as_input();
}

  if (header_slave[0] == 0x02) {
    /* SPI is ready */
    if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {
  
      /*  Buffer is big enough */
      //if (Nb_bytes1 > 0) {
      //  HAL_SPI_TransmitReceive(hspi, data1, read_char_buf, Nb_bytes1, TIMEOUT_DURATION);
      //}
      //if (Nb_bytes2 > 0) {
      //  HAL_SPI_TransmitReceive(hspi, data2, read_char_buf, Nb_bytes2, TIMEOUT_DURATION);
      //}
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
  //HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  gpio_write(&gpio_pin_CS, 1);
			
  Enable_SPI_IRQ();
    
  return result;
}
      
/**
 * @brief  Set in Output mode the IRQ.
 * @param  None
 * @retval None
 */
void set_irq_as_output(void)
{
#ifdef CUBE
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Pull IRQ high */
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  HAL_GPIO_WritePin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN, GPIO_PIN_SET);
#endif
	
	//SO: ???????
	/*Init IRQ for EXTI Interrupt*/	
	gpio_irq_init(&irq_exti, PIN_SPI_IRQ, EXTI_irq_handler,(uint32_t)BNRG_SPI_INSTANCE);
	//gpio_irq_set(&irq_exti, IRQ_RISE, 1);
	//gpio_init(&gpio_pin_A0, PIN_SPI_IRQ);
	gpio_dir(&gpio_pin_A0, PIN_OUTPUT);
	gpio_mode(&gpio_pin_A0, PullNone);
	gpio_write(&gpio_pin_A0, 1);
	
}

/**
 * @brief  Set the IRQ in input mode.
 * @param  None
 * @retval None
 */
void set_irq_as_input(void)
{
#ifdef CUBE
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* IRQ input */  
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = BNRG_SPI_IRQ_MODE;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Alternate = BNRG_SPI_IRQ_ALTERNATE;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pull = BNRG_SPI_IRQ_PULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
#endif
	
	//SO: ???????
	/*Init IRQ for EXTI Interrupt*/	
	gpio_irq_init(&irq_exti, PIN_SPI_IRQ, EXTI_irq_handler,(uint32_t)BNRG_SPI_INSTANCE);
	gpio_irq_set(&irq_exti, IRQ_RISE, 1);
	gpio_init(&gpio_pin_A0, PIN_SPI_IRQ);
	gpio_mode(&gpio_pin_A0, PullDown);
	gpio_dir(&gpio_pin_A0, PIN_INPUT);
	gpio_write(&gpio_pin_A0, 0);
	
	gpio_mode(&gpio_pin_A0, PullNone);

}

/**
 * @brief  Utility function for delay
 * @param  None
 * @retval None
 * NOTE: TODO: implement with clock-independent function.
 */
static void us150Delay(void)
{
#if SYSCLK_FREQ == 4000000
  for(volatile int i = 0; i < 35; i++)__NOP();
#elif SYSCLK_FREQ == 32000000
  for(volatile int i = 0; i < 420; i++)__NOP();
#elif SYSCLK_FREQ == 84000000
  for(volatile int i = 0; i < 1125; i++)__NOP();
#else
#error Implement delay function.
#endif    
}

/**
 * @brief  Enable SPI IRQ.
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{
  //HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);  
	gpio_irq_enable(&irq_exti);
}

/**
 * @brief  Disable SPI IRQ.
 * @param  None
 * @retval None
 */
void Disable_SPI_IRQ(void)
{ 
  //HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
	gpio_irq_disable(&irq_exti);
}

/**
 * @brief  Clear Pending SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
  //HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
	gpio_irq_free(&irq_exti);
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{  
  //__HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);  //SO: ???????
	//gpio_irq_free(&irq_exti);
}

/**
* @}
*/

/**
 * @}
 */

/**
 * @}
 */
   
/**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
