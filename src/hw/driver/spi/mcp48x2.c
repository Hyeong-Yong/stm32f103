/*
 * dac.c
 *
 *  Created on: 2022. 4. 19.
 *      Author: HYJH
 */

#include "mcp48x2.h"
#include "gpio.h"
#include "spi.h"
#include "cli.h"

#ifdef _USE_HW_MCP48x2

#define REF_VOLTAGE_MV 2048

#if defined(DAC_TYPE_MCP4822)
 #define MAX_OUT_VALUE (4095)
 #define SHIFT_BITS_CNT (0)
#elif defined(DAC_TYPE_MCP4812)
 #define MAX_OUT_VALUE (1023)
 #define SHIFT_BITS_CNT (2)
#elif defined(DAC_TYPE_MCP4802)
 #define MAX_OUT_VALUE (255)
 #define SHIFT_BITS_CNT (4)
#else
 #error "Please, define DAC type"
#endif




#define _DEF_MSP48x2_CS _DEF_GPIO1
#define _DEF_MSP48x2_LDAC _DEF_GPIO1
const uint8_t spi_ch = _DEF_SPI1;


#ifdef _USE_HW_CLI
static void cliMcp48x2(cli_args_t *args);
#endif

mcp48x2_config_t mcp48x2_config;

static void mcp48x2_CsWrite(bool value);
static void mcp48x2_LatchWrite();
static void mcp48x2_SpiTransfer16(mcp48x2_config_t* config, uint16_t tx_data);


bool mcp48x2_Init()
{
  bool ret= true;

  ret =spiBegin(spi_ch);

  mcp48x2_CsWrite(_DEF_RESET);
  mcp48x2_setConfig(&mcp48x2_config, DAC_ENABLE, DAC_SELECT_A, DAC_GAIN_DOUBLE);


#ifdef _USE_HW_CLI
  cliMcp48x2("mcp4822", climcp48x2);
#endif

  return ret;
}

void mcp48x2_CsWrite(bool value){
	gpioPinWrite(_DEF_MSP48x2_CS, value);
};

void mcp48x2_LatchWrite(){
	gpioPinLatch(_DEF_MSP48x2_LDAC, 100);
};

void mcp48x2_SpiTransfer16(mcp48x2_config_t* config, uint16_t tx_data){
	  mcp48x2_CsWrite(_DEF_RESET);
	  spiTransfer16(spi_ch, tx_data, 100);
	  mcp48x2_CsWrite(_DEF_SET);
	  mcp48x2_LatchWrite();
};

/*
 ****************************** Configuration********************************************************
 */
void mcp48x2_setConfig(mcp48x2_config_t* config, mcp48x2_enable_t enable, mcp48x2_sel_AorB_t ch, mcp48x2_gain_t gain)
{
  uint16_t tx_data =0;

  tx_data |= enable <<SHDN_BIT_POS; 	      // SHDN bit [12]
  config->enable = enable;


	if (ch == DAC_SELECT_A)
	{
		config->enable = enable;
		config->sel_A = ch;
		config->gain_A = gain;
	}
	else if (ch == DAC_SELECT_B)
	{
		config->enable = enable;
		config->sel_B = ch;
		config->gain_B = gain;
	}

  mcp48x2_SpiTransfer16(config, tx_data);
}


//TODO: setEnable, selChannel, selGain

/*****************************************************************************************************/

bool mcp48x2_setVoltage(mcp48x2_sel_AorB_t DAC_select_ch, float voltage)
{
	uint16_t tx_data = 0;
    uint8_t gain = 1; // Default gain is the DAC_GAIN_SINGLE (X1)

    if (DAC_select_ch == DAC_SELECT_A){
    	tx_data |= mcp48x2_config.enable;
    	tx_data |= mcp48x2_config.sel_A;
    	tx_data |= mcp48x2_config.gain_A;
    	if (mcp48x2_config.gain_A == DAC_GAIN_DOUBLE){ //(X2)
    		gain = 2;
    	}
    }
    else if (DAC_select_ch == DAC_SELECT_B){
        tx_data |= mcp48x2_config.enable;
        tx_data |= mcp48x2_config.sel_B;
        tx_data |= mcp48x2_config.gain_B;
    	if (mcp48x2_config.gain_B == DAC_GAIN_DOUBLE){
    		gain = 2;
    	}
    }

    uint16_t DAC_input = (voltage * MAX_OUT_VALUE)/(REF_VOLTAGE_MV*gain);

    if (DAC_input > MAX_OUT_VALUE)
    {
    	return false;
    }

    tx_data |= DAC_input << SHIFT_BITS_CNT; // (e.g., MCP4822) Data bit   [0-11], value have to be lower than 4095 (12bit)


    mcp48x2_SpiTransfer16(&mcp48x2_config, tx_data);


	if (DAC_select_ch == DAC_SELECT_A)
	{
		mcp48x2_config.ch_A_voltage = voltage;
	}
	else if (DAC_select_ch == DAC_SELECT_B)
	{
		mcp48x2_config.ch_B_voltage = voltage;
	}



    return true;
}

#ifdef _USE_HW_CLI
void cliMcp48x2(cli_args_t *args){

	  bool ret = false;

	  if (args->argc == 3 && args->isStr(0, "enable") == true)
	    {
	      uint8_t ch;
	      uint32_t toggle_time;
	      uint32_t pre_time;

	      led_ch = (uint8_t)args->getData(1);
	      toggle_time = (uint32_t)args->getData(2);

	      if (led_ch >0)
	        {
	          led_ch--;
	        }
	      pre_time=millis();
	      while (cliKeepLoop())
	        {
	          if (millis()-pre_time >= toggle_time)
	            {
	              pre_time = millis();
	              ledToggle(led_ch);
	            }
	        }
	    }


	  if (args->argc == 2 && args->isStr(0, "switch") == true) // led toggle ch on
	    {
	      uint8_t led_ch;

	      led_ch = (uint8_t)args->getData(1);

	      if (led_ch >0)
	        {
	          led_ch--;
	        }
	      ledToggle(led_ch);
	    }


	  if (ret != true)
	    {
	      cliPrintf("led toggle ch[1~%d] time_ms\n", LED_MAX_CH);
	      cliPrintf("led switch ch[1~%d] \n", LED_MAX_CH);
	    }
}
#endif

#endif
