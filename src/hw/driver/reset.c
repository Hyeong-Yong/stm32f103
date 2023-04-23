/*
 * reset.c
 *
 *  Created on: 2022. 1. 3.
 *      Author: HYJH
 */


#include "reset.h"
#include "rtc.h"


#ifdef _USB_HW_RESET

static uint32_t reset_count=0;

bool resetInit(void)
{
  bool ret= true;

// if reset pin is set,
  if (RCC->CSR & (1<<26))
    {
      rtcBackupRegWrite(1, rtcBackupRegRead(1)+1);
      delay(500);
      reset_count = rtcBackupRegRead(1);
    }
  // register initialization to 0
  rtcBackupRegWrite(1, 0);

  return ret;
}

uint32_t resetGetCount(void)
{
  return reset_count;
}



#endif
