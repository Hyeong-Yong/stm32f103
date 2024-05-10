/*
 * hw.c
 *
 *  Created on: Dec 31, 2021
 *      Author: HYJH
 */

#include "hw.h"

void hwInit(void)
{
  bspInit();
  cliInit();
//  resetInit();
  usbInit();
//  rtcInit();
  tim1Init();
  gpioInit();
  ledInit();
//  motorInit();
  spiInit();
  uartInit();
//  flashInit();
}
