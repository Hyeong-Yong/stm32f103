/*
 * spi.c
 *
 *  Created on: 2022. 4. 18.
 *      Author: HYJH
 */
#include "spi.h"

#ifdef _USE_HW_SPI


typedef struct
{
  bool is_open;
  bool is_tx_done;

  void (*func_tx)(void);

  SPI_HandleTypeDef *h_spi;
  DMA_HandleTypeDef *h_dma;
} spi_t;


spi_t spi_tbl[SPI_MAX_CH];
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;


bool spiInit(void)
{
  bool ret = true;

  for (int i =0; i<SPI_MAX_CH; i++)
    {
     spi_tbl[i].is_open    = false;
     spi_tbl[i].is_tx_done = false;
     spi_tbl[i].func_tx    = NULL;
    }

  return ret;
}

bool spiBegin(uint8_t ch)
{
  bool ret = false;
  spi_t *p_spi = &spi_tbl[ch];

  switch (ch)
  {
    case _DEF_SPI1:
      p_spi->h_spi = &hspi1;
      p_spi->h_dma = &hdma_spi1_tx;

      hspi1.Instance = SPI1;
      hspi1.Init.Mode = SPI_MODE_MASTER;
      hspi1.Init.Direction = SPI_DIRECTION_2LINES;
      hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
      hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
      hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
      hspi1.Init.NSS = SPI_NSS_SOFT;
      hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
      hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
      hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
      hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
      hspi1.Init.CRCPolynomial = 10;

      if (HAL_SPI_Init(&hspi1) == HAL_OK)
      {
        p_spi->is_open=true;
        ret= true;
      }
      break;

  }
  return ret;
}

void spiSetDataMode(uint8_t ch, uint8_t dataMode)
{
  spi_t *p_spi = &spi_tbl[ch];

  if (p_spi->is_open==false)
    {
      return;
    }

  switch (dataMode)
  {
    // CPOL=0, CPHA=0
    case SPI_MODE0:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_LOW;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_1EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;

      // CPOL=0, CPHA=1
    case SPI_MODE1:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_LOW;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_2EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;

      // CPOL=1, CPHA=0
    case SPI_MODE2:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_1EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;

      // CPOL=1, CPHA=1
    case SPI_MODE3:
      p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
      p_spi->h_spi->Init.CLKPhase    = SPI_PHASE_2EDGE;
      HAL_SPI_Init(p_spi->h_spi);
      break;
  }
}

void spiSetBitWidth(uint8_t ch, uint8_t bit_width)
{
  spi_t* p_spi = &spi_tbl[ch];

  if (p_spi->is_open==false) return;

  if (bit_width == 16)
    {
      p_spi->h_spi->Init.DataSize = SPI_DATASIZE_16BIT;
    }
  else
    {
  p_spi->h_spi->Init.DataSize = SPI_DATASIZE_8BIT;
    }
  HAL_SPI_Init(p_spi->h_spi);
}

uint8_t  spiTransfer8(uint8_t ch, uint8_t data, uint32_t timeout) //polling
{
  uint8_t ret=0;

  spi_t *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false) return ret;

  HAL_SPI_TransmitReceive(p_spi->h_spi, &data, &ret, 1, timeout);

  return ret;
}

uint16_t spiTransfer16(uint8_t ch, uint16_t data, uint32_t timeout)
{
  uint16_t ret =0;
  uint8_t tBuf[2];
  uint8_t rBuf[2];

  spi_t* p_spi = &spi_tbl[ch];


  if(p_spi->is_open == false) return ret;

  if(p_spi->h_spi->Init.DataSize == SPI_DATASIZE_8BIT)
    {
      tBuf[1] = (uint8_t)data;
      tBuf[0] = (uint8_t)(data>>8); //MSB first
      HAL_SPI_TransmitReceive(p_spi->h_spi, (uint8_t *)&tBuf, (uint8_t *)&rBuf, 2, timeout);

      ret = rBuf[0];
      ret <<= 8;
      ret += rBuf[1];
    }
  else
    {
      HAL_SPI_TransmitReceive(p_spi->h_spi, (uint8_t *)&data, (uint8_t*)&ret, 1, timeout);
    }
  return ret;
}

void spiDmaTxStart(uint8_t ch, uint8_t *p_buf, uint32_t length)
{
  spi_t* p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false) return;

  p_spi->is_tx_done = false;

  HAL_SPI_Transmit_DMA(p_spi->h_spi, p_buf, length);
}

void spiDmaTxTransfer(uint8_t ch, void *buf, uint32_t length, uint32_t timeout)
{
  uint32_t t_time;

  spiDmaTxStart(ch, (uint8_t *)buf, length);

  t_time = millis();

  if (timeout ==0) return;

  while (1)
    {
      if(spiDmaTxIsDone(ch))
	{
	  break;
	}
      if((millis()-t_time) > timeout)
	{
	  break;
	}
    }
}

bool spiDmaTxIsDone(uint8_t ch)
{
  spi_t *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false) return true;

  return p_spi->is_tx_done;
}

void spiAttachTxInterrupt(uint8_t ch, void (*func)())
{
  spi_t *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false) return;

  p_spi->func_tx = func; //register callbackFunction
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  spi_t  *p_spi;

  if(hspi->Instance == spi_tbl[_DEF_SPI1].h_spi->Instance)
    {
      p_spi = &spi_tbl[_DEF_SPI1];

      p_spi->is_tx_done = true;

      if (p_spi->func_tx != NULL)
	{
	  p_spi->func_tx();
	}
    }
}


void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */
      __HAL_RCC_DMA1_CLK_ENABLE();

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI1 DMA Init */
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA1_Channel3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi1_tx);

    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_7);

    /* SPI1 DMA DeInit */
    HAL_DMA_DeInit(spiHandle->hdmatx);

    /* SPI1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI1_IRQn);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}



#endif
