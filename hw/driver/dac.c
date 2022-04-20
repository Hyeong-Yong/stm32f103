/*
 * dac.c
 *
 *  Created on: 2022. 4. 19.
 *      Author: HYJH
 */

#include "dac.h"
#include "gpio.h"
#include "spi.h"

#ifdef _USE_HW_DAC

typedef enum  // SHDN bit   [12], (if 0, not available), (if 1, Vout is available)
{
  DAC_DISABLE= 0,
  DAC_ENABLE
}SHDN_bit;
typedef enum  // GA bit    [13], (if 0, Vout = 2*Vref*data/4096), (if 1, Vout = Vref*data/4096)
{
  DAC_GAIN_DOUBLE= 0,
  DAC_GAIN_SINGLE
}GA_bit;
typedef enum  // A/B bit   [15], (if 0, select DAC_A), (if 1, select DAC_B)
{
  DAC_SELECT_A=0,
  DAC_SELECT_B
}A_B_bit;


static uint16_t tx_data = 0;

bool DACInit()
{
  bool ret= true;

  return ret;
}

void setConfigureBit()
{
  tx_data |= DAC_ENABLE <<12; 	      // SHDN bit [12]
  tx_data |= DAC_GAIN_DOUBLE <<13;    // GA bit   [13]
  tx_data |= DAC_SELECT_A <<15;       // A/B bit  [15]
}

bool setVoltageDAC_Polling(float voltage)
{
  uint16_t int_voltage=(uint16_t)(voltage*1000);
    if (int_voltage > 4096)
    {
	return false;
    }

    tx_data  = 0;
    tx_data  = int_voltage;   // Data bit   [0-11], value have to be lower than 4095 (12bit)
    setConfigureBit();

  gpioPinWrite(_DEF_DAC_CS, false);
  spiTransfer16(_DEF_SPI1, tx_data, 100);
  gpioPinWrite(_DEF_DAC_CS, true);
  gpioPinLatch(_DEF_DAC_LDAC);
  return true;
}

bool setVoltageDAC_DMA(float voltage)
{
  uint16_t int_voltage=(uint16_t)(voltage*1000);
  uint8_t tBuf[2];

    if (int_voltage > 4096)
    {
	return false;
    }

  tx_data  = 0;
  tx_data  = int_voltage;   // Data bit   [0-11], value have to be lower than 4095 (12bit)
  setConfigureBit();
  tBuf[0]=tx_data >>8;
  tBuf[1]=tx_data;

  gpioPinWrite(_DEF_DAC_CS, false);
  spiDmaTxStart(_DEF_SPI1, (uint8_t *)&tBuf, 2);
  gpioPinWrite(_DEF_DAC_CS, true);
  gpioPinLatch(_DEF_DAC_LDAC);


  return true;
}

#endif
