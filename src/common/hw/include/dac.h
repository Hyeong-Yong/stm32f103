/*
 * dac.h
 *
 *  Created on: 2022. 4. 19.
 *      Author: HYJH
 */

#ifndef SRC_BSP_DAC_H_
#define SRC_BSP_DAC_H_

#include "hw_def.h"

#ifdef _USE_HW_DAC

bool DACInit();
void setConfigureBit();
bool setVoltageDAC_Polling(float voltage);
bool setVoltageDAC_DMA(float voltage);

#endif

#endif /* SRC_BSP_DAC_H_ */
