/*
 * dac.h
 *
 *  Created on: 2022. 4. 19.
 *      Author: HYJH
 */

#ifndef SRC_BSP_DAC_H_
#define SRC_BSP_DAC_H_

#include "hw_def.h"

#ifdef _USE_HW_MCP48x2


#define SHDN_BIT_POS 12
typedef enum  // SHDN(shutdown) bit   [12], (if 0, not available), (if 1, Vout is available)
{
  DAC_DISABLE= 0,
  DAC_ENABLE
}mcp48x2_enable_t;

#define GAIN_BIT_POS 13
typedef enum  // GA bit    [13], (if 0, Vout = 2*Vref*data/4096), (if 1, Vout = Vref*data/4096)
{
  DAC_GAIN_DOUBLE= 0,
  DAC_GAIN_SINGLE
}mcp48x2_gain_t;

#define CHANNEL_BIT_POS 15
typedef enum  // A/B bit   [15], (if 0, select DAC_A), (if 1, select DAC_B)
{
  DAC_SELECT_A=0,
  DAC_SELECT_B
}mcp48x2_sel_AorB_t;

typedef struct {
	//Configuration
	mcp48x2_enable_t enable;
	mcp48x2_gain_t gain_A;
	mcp48x2_gain_t gain_B;
	mcp48x2_sel_AorB_t sel_A;
	mcp48x2_sel_AorB_t sel_B;
	//Get Values
	float ch_A_voltage; //DAC values in A and B channel
	float ch_B_voltage;
}mcp48x2_config_t;

bool mcp48x2_init();
void mcp48x2_setConfig(mcp48x2_config_t* config, mcp48x2_enable_t enable, mcp48x2_sel_AorB_t ch, mcp48x2_gain_t gain);
bool mcp48x2_setVoltageDAC_Polling(uint8_t ch, float voltage);
bool mcp48x2_setVoltageDAC_DMA(uint8_t ch, float voltage);

#endif

#endif /* SRC_BSP_DAC_H_ */
