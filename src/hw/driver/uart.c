/*
 * uart.c
 *
 *  Created on: 2022. 1. 1.
 *      Author: HYJH
 */

#include "uart.h"
#include "cdc.h"
#include "qbuffer.h"



static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[128];


UART_HandleTypeDef huart1;
HAL_StatusTypeDef status;
DMA_HandleTypeDef hdma_usart1_rx;


bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH;i++)
    {
      is_open[i] = false;
    }
  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  switch(ch)
  {
    case _DEF_UART1: // USB는 가성 포트
    is_open[ch] = true;
    ret = true;

    case _DEF_UART2: //USART2는 물리적인 설정이 필요
      huart1.Instance          = USART1;
      huart1.Init.BaudRate     = baud;
      huart1.Init.WordLength   = UART_WORDLENGTH_8B;
      huart1.Init.StopBits     = UART_STOPBITS_1;
      huart1.Init.Parity       = UART_PARITY_NONE;
      huart1.Init.Mode         = UART_MODE_TX_RX;
      huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
      huart1.Init.OverSampling = UART_OVERSAMPLING_16;

      HAL_UART_DeInit(&huart1);

      qbufferCreate(&qbuffer[ch], &rx_buf[0], 256); // 256 길이의 &rx_buf[0](=rx_buf)을 주소를 갖는 qbuffer 구조체 생성

        __HAL_RCC_DMA1_CLK_ENABLE();
        HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);



      if (HAL_UART_Init(&huart1) != HAL_OK)
      {
        ret = false;
      }
      else
        {
          ret= true;
          is_open[ch]=true;

          if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)&rx_buf[0], 256) != HAL_OK)
          {
            ret = false;
          }

          qbuffer[ch].head = qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR; //256-256= 0, 256-255= 1, ...
          qbuffer[ch].tail = qbuffer[ch].head; //Flash
        }
    break;
  }
  return ret;
}


uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret= cdcAvailable();
      break;

    case _DEF_UART2:
      qbuffer[ch].head=qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR;
      ret = qbufferAvailable(&qbuffer[ch]);
      break;
  }
  return ret;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret=0;
  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcRead();
      break;

    case _DEF_UART2:
      qbufferRead(&qbuffer[ch], &ret ,1);
      break;

  }
  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret= 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret =cdcWrite(p_data, length);
      break;
    case _DEF_UART2:
      status = HAL_UART_Transmit(&huart1, p_data, length, 100);
      if (status==HAL_OK)
        {
          ret= length;
        }
      break;
  }
  return ret;
}

uint32_t uartPrintf(uint8_t ch, char*fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;
  va_start(args, fmt);


  len = vsnprintf(buf, 256, fmt, args);
  ret=uartWrite(ch, (uint8_t *)buf, len);


  va_end(args);
  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
  ret = cdcGetBaud();
  case _DEF_UART2:
    ret= huart1.Init.BaudRate;
  break;
  }
  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

bool uartClose(uint8_t ch)
{
  return true;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance ==USART1)
    {}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if 0
  if (huart->Instance ==USART1) // 어떤 UART 인터럽트(예: UART2, UART3 등)가 걸리면 RxCpItCallback으로 들어오니까 USART1만 작동하도록
    {
      qbufferWrite(&qbuffer[_DEF_UART2], &rx_data[_DEF_UART2], 1);

      HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx_data[_DEF_UART2], 1);
    }
#endif

}






void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

