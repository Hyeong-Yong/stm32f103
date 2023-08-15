/*
 * ap.c
 *
 *  Created on: Dec 31, 2021
 *      Author: HYJH
 */


#include "ap.h"


void apInit(void)
{
//  uartOpen(_DEF_UART1, 57600); // USB_CDC
  uartOpen(_DEF_UART2, 57600);  // UART

  cliOpen(_DEF_UART1, 57600);
//  cliOpenLog(_DEF_UART2, 57600);

// motorOpen(_DEF_MOTOR1, 57600);
  HAL_TIM_Base_Start(&htim1);

  //  spiBegin(_DEF_SPI1);
  spiBegin(_DEF_SPI2);
}



void apMain(void)
{
  uint32_t pre_time;
  pre_time=millis();


  float voltage=0;

  while(1)
    {
      if (millis()-pre_time >=500)
        {
          pre_time=millis();
          //setVoltageDAC_Polling(voltage);   //setVoltageDAC_DMA(voltage);
          voltage+=0.2;
          if (voltage > 4)
            {
              voltage= 0;
            }
        }


//      motorGUIRun();
      cliMain();

    }
}

