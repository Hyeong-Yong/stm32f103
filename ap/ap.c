/*
 * ap.c
 *
 *  Created on: Dec 31, 2021
 *      Author: HYJH
 */


#include "ap.h"


void apInit(void)
{
  uartOpen(_DEF_UART1, 57600); // USB_CDC
  uartOpen(_DEF_UART2, 57600);  // UART

  cliOpen(_DEF_UART1, 57600);
//  cliOpenLog(_DEF_UART2, 57600);

 motorOpen(_DEF_MOTOR1, 57600);
  HAL_TIM_Base_Start(&htim1);
}

void apMain(void)
{
  uint32_t pre_time;
  pre_time=millis();

  while(1)
    {
      if (millis()-pre_time >=2000)
        {
          pre_time=millis();
//          ledToggle(_DEF_LED1);
//          uartPrintf(_DEF_UART2, "\n");
        }

/*
      if (uartAvailable(_DEF_UART2) > 0)
      {
        uint8_t rx_data;
        rx_data = uartRead(_DEF_UART2);

        uartPrintf(_DEF_UART2, "Rx : 0x%X\n", rx_data);
      }
*/


      motorGUIRun();
      cliMain();

    }
}

