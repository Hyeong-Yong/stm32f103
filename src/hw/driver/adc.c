/*
 * adc.c
 *
 *  Created on: Apr 30, 2024
 *      Author: hwang
 */


#include "adc.h"
#include "cli.h"
#include "uart.h"

#ifdef _USE_HW_CLI
static void cliADC(cli_args_t *args);
#endif

#ifdef _USE_HW_ADC

typedef struct
{
  bool                    is_init;
  ADC_HandleTypeDef*	  hADCx;
} adc_tbl_t;


#define ADC_BUF_SIZE 5

static adc_tbl_t adc_tbl[ADC_MAX_CH];

static volatile uint32_t adc_buf[ADC_BUF_SIZE];

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

float temp;
#define V25 1.43 // from datasheet
#define VSENSE 3.3/4096 // VSENSE value
#define Avg_Slope .0043 // 4.3mV from datasheet
static float get_temp (uint32_t variable)
{
 return (((V25 - (variable*VSENSE)) / Avg_Slope) + 25); // formula from datasheet
}

void adcInit()
{

  for (int i = 0; i < ADC_MAX_CH; i++)
    {
      adc_tbl[i].is_init = false;
      adc_tbl[i].hADCx = &hadc1;

      /* USER CODE BEGIN ADC1_Init 0 */

      /* USER CODE END ADC1_Init 0 */

      ADC_ChannelConfTypeDef sConfig = {0};

      /* USER CODE BEGIN ADC1_Init 1 */

      /* USER CODE END ADC1_Init 1 */

      /** Common config
      */
      hadc1.Instance = ADC1;
      hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
      hadc1.Init.ContinuousConvMode = ENABLE;
      hadc1.Init.DiscontinuousConvMode = DISABLE;
      hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
      hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
      hadc1.Init.NbrOfConversion = 5;
      if (HAL_ADC_Init(&hadc1) == HAL_OK)
      {
          adc_tbl[i].is_init     = true;
      }

      sConfig.Channel = ADC_CHANNEL_5;
      sConfig.Rank = ADC_REGULAR_RANK_1;
      sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      /** Configure Regular Channel
      */
      sConfig.Channel = ADC_CHANNEL_7;
      sConfig.Rank = ADC_REGULAR_RANK_2;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      /** Configure Regular Channel
      */
      sConfig.Channel = ADC_CHANNEL_8;
      sConfig.Rank = ADC_REGULAR_RANK_3;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      /** Configure Regular Channel
      */
      sConfig.Channel = ADC_CHANNEL_9;
      sConfig.Rank = ADC_REGULAR_RANK_4;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      /** Configure Regular Channel
      */
      sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
      sConfig.Rank = ADC_REGULAR_RANK_5;
      sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

    }


#ifdef _USE_HW_CLI
	cliAdd("adc", cliADC);
#endif
}

void ADCstart_DMA(uint8_t ch)
{
  HAL_ADC_Start_DMA(adc_tbl[ch].hADCx, (uint32_t*)&adc_buf, ADC_BUF_SIZE);
}


void ADCstop_DMA(uint8_t ch)
{
  HAL_ADC_Stop_DMA(adc_tbl[ch].hADCx);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  if(adcHandle->Instance==ADC1)
	  {
	  /* USER CODE BEGIN ADC1_MspInit 0 */

	  /* USER CODE END ADC1_MspInit 0 */

		    /* DMA controller clock enable */
		    __HAL_RCC_DMA1_CLK_ENABLE();


	    /* ADC1 clock enable */
	    __HAL_RCC_ADC1_CLK_ENABLE();

	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	    /**ADC1 GPIO Configuration
	    PA5     ------> ADC1_IN5
	    PA7     ------> ADC1_IN7
	    PB0     ------> ADC1_IN8
	    PB1     ------> ADC1_IN9
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
	    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    /* ADC1 DMA Init */
	    /* ADC1 Init */
	    hdma_adc1.Instance = DMA1_Channel1;
	    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	    hdma_adc1.Init.Mode = DMA_CIRCULAR;
	    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
	    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
	    {
	      Error_Handler();
	    }

	    __HAL_LINKDMA(adcHandle,DMA_Handle,  hdma_adc1);

	    /* DMA1_Channel1_IRQn interrupt configuration */
	    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	  /* USER CODE BEGIN ADC1_MspInit 1 */

	  /* USER CODE END ADC1_MspInit 1 */
	  }
}
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

	  if(adcHandle->Instance==ADC1)
	  {
	  /* USER CODE BEGIN ADC1_MspDeInit 0 */

	  /* USER CODE END ADC1_MspDeInit 0 */
	    /* Peripheral clock disable */
	    __HAL_RCC_ADC1_CLK_DISABLE();

	    /**ADC1 GPIO Configuration
	    PA5     ------> ADC1_IN5
	    PA7     ------> ADC1_IN7
	    PB0     ------> ADC1_IN8
	    PB1     ------> ADC1_IN9
	    */
	    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_7);

	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);

	    /* ADC1 DMA DeInit */
	    HAL_DMA_DeInit(adcHandle->DMA_Handle);
	  /* USER CODE BEGIN ADC1_MspDeInit 1 */

	  /* USER CODE END ADC1_MspDeInit 1 */
	  }
}


uint8_t checksumPacket(uint8_t* packet, uint32_t packet_length)
{
  bool ret = false;
  uint32_t sum = 0;
  for (int i = 0; i < packet_length-1; i++)
    {
      sum += packet[i];
    }

  sum = sum & 0xFF;
  sum = (~sum + 1) & 0xFF;

  return (uint8_t)sum;
}



static void cliADC(cli_args_t* args){
	  bool ret = false;
	  if (args->argc == 1 && args->isStr(0, "start") == true)
	    {

		  const uint8_t Header = 0x10;
		  const uint8_t type1 = 0x21;
		  const uint8_t type2 = 0x22;
		  const uint8_t type3 = 0x23;
		  const uint8_t type4 = 0x24;
		  uint8_t packet[5];

		  while(cliKeepLoop()){
			  ADCstart_DMA(_DEF_ADC1);
			  // packet[0]   Header (0x10)
			  // packet[1]   Type (APD1, APD2, APD3, Temp)
			  // packet[2,3] upp_value, lower value
			  // packet[4]   CheckSum

			  packet[0]= Header;
			  for (int i=0 ; i<4;i++){
				  packet[1] = type1+i;
				  packet[2] = adc_buf[i];
				  packet[3] = adc_buf[i]>>8;
				  packet[4] = checksumPacket(packet, 5);
				  uartWrite(_DEF_UART1, packet, 5);
			  }
			  delay(1000);


			  //temp = get_temp(adc_buf[0]);

			  //cliPrintf("temperature: %.2f\n", temp);
			  ADCstop_DMA(_DEF_ADC1);
		  }
	    }


	  if (ret != true)
	    {
	      cliPrintf("adc start\n");
	    }
}

#endif
