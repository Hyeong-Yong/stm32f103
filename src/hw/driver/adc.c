/*
 * adc.c
 *
<<<<<<< HEAD
 *  Created on: Apr 30, 2024
 *      Author: hwang
 */


#include "adc.h"

#ifdef _USE_HW_ADC

typedef struct
{
  bool                    is_init;
  ADC_HandleTypeDef*	  hADCx;
  uint32_t                adc_channel1; // PA5
  uint32_t                adc_channel2; // PA7
  uint32_t                adc_channel3; // PB0
  uint32_t                adc_channel4; // PB1
  uint32_t                adc_channel5; // Temperature sensor
} adc_tbl_t;

#define ADC_BUF_SIZE 5

static adc_tbl_t adc_tbl[ADC_MAX_CH];

static volatile uint16_t adc_buf[ADC_BUF_SIZE];

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

float temp;
float V25= 1.43;
float Avg_Slope = .0043;
float VSENSE = 3.3/4096;

float get_temp (uint32_t variable){
	VSENSE = VSENSE * variable;
	return ( (V25- VSENSE)/Avg_Slope+25 );
}


void adcInit()
{

  for (int i = 0; i < ADC_MAX_CH; i++)
    {
      adc_tbl[i].is_init = false;

      ADC_ChannelConfTypeDef sConfig ={0};

      /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
       */

      hadc1.Instance = ADC1;
      hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
      hadc1.Init.ContinuousConvMode = ENABLE;
      hadc1.Init.DiscontinuousConvMode = DISABLE;
      hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
      hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
      hadc1.Init.NbrOfConversion = 5;

      if (HAL_ADC_Init (&hadc1) == HAL_OK)
	{
	  adc_tbl[i].is_init = true;
	}

      sConfig.Channel = ADC_CHANNEL_5;
      sConfig.Rank = ADC_REGULAR_RANK_1;
      sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      sConfig.Channel = ADC_CHANNEL_7;
      sConfig.Rank = ADC_REGULAR_RANK_2;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      sConfig.Channel = ADC_CHANNEL_8;
      sConfig.Rank = ADC_REGULAR_RANK_3;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      sConfig.Channel = ADC_CHANNEL_9;
      sConfig.Rank = ADC_REGULAR_RANK_4;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
      sConfig.Rank = ADC_REGULAR_RANK_5;
      sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }

      adc_tbl[i].is_init     = true;
      adc_tbl[i].hADCx       = &hadc1;
      adc_tbl[i].adc_channel1 = ADC_CHANNEL_5;
      adc_tbl[i].adc_channel2 = ADC_CHANNEL_7;
      adc_tbl[i].adc_channel3 = ADC_CHANNEL_8;
      adc_tbl[i].adc_channel4 = ADC_CHANNEL_9;
      adc_tbl[i].adc_channel5 = ADC_CHANNEL_TEMPSENSOR;
    }
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

	    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

        /* DMA interrupt init */
        /* DMA2_Stream0_IRQn interrupt configuration */

	    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  }
}


void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA5     ------> ADC1_IN5
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
  }

}


#endif
=======
 *  Created on: 2024. 4. 29.
 *      Author: User
 */


>>>>>>> 53b375ada1cfcbb6ba7c7f64104a331a010ce9e5
