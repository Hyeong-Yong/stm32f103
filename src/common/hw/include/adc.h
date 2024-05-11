/*
 * adc.h
 *
 *  Created on: Apr 30, 2024
 *      Author: hwang
 */

#ifndef SRC_COMMON_HW_INCLUDE_ADC_H_
#define SRC_COMMON_HW_INCLUDE_ADC_H_

#include "hw_def.h"


void adcInit();
void ADCstart_DMA(uint8_t ch);
void ADCstop_DMA(uint8_t ch);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);


#endif /* SRC_COMMON_HW_INCLUDE_ADC_H_ */
