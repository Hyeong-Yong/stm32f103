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
  rtcInit();

  resetInit();
  ledInit();
//  motorInit();
  usbInit();
  uartInit();
  flashInit();
}


