/*
 * motor_uart.c
 *
 *  Created on: 2022. 4. 11.
 *      Author: HYJH
 */

#include "motor/motor_uart.h"
#include "uart.h"

#ifdef _USE_HW_MOTOR


static bool open(uint8_t ch, uint32_t baud);
static bool close(uint8_t ch);
static uint32_t available(uint8_t ch);
static uint32_t write(uint8_t ch, uint8_t* p_data, uint32_t length);
static uint8_t read(uint8_t ch);
static bool flush(uint8_t ch);



bool motorUartDriver(motor_driver_t* p_driver)
{
  p_driver->isInit    = true;
  p_driver->open      = open;
  p_driver->close     = close;
  p_driver->write     = write;
  p_driver->read      = read;
  p_driver->available = available;
  p_driver->flush     = flush;
  return true;
}
				    //_DEF_MOTOR1=> UART2,  _DEF_MOTOR2=> UART1
static const uint8_t motor_ch_tbl[]={_DEF_UART2, _DEF_UART1};

bool open(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  switch (ch)
  {
  case _DEF_MOTOR1:
    ret=uartOpen(motor_ch_tbl[ch], baud);
    break;
  case _DEF_MOTOR2:
    ret=uartOpen(motor_ch_tbl[ch], baud);
    break;
  }
  return ret;
}
bool close(uint8_t ch)
{
  bool ret = false;
  switch (ch)
  {
    case _DEF_MOTOR1:
    ret=uartClose(motor_ch_tbl[ch]);
    break;
  case _DEF_MOTOR2:
    ret=uartClose(motor_ch_tbl[ch]);
    break;
  }
  return ret;
}
uint32_t available(uint8_t ch)
{
  uint32_t ret=0;
  switch (ch)
  {
    case _DEF_MOTOR1:

    ret=uartAvailable(motor_ch_tbl[ch]);
    break;
  case _DEF_MOTOR2:
    ret=uartAvailable(motor_ch_tbl[ch]);
    break;
  }
    return ret;
}
uint32_t write(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret=0;

  switch (ch)
  {
  case _DEF_MOTOR1:
    ret=uartWrite(motor_ch_tbl[ch], p_data, length);
    break;
  case _DEF_MOTOR2:
    ret=uartWrite(motor_ch_tbl[ch], p_data, length);
    break;
  }
  return ret;
}

uint8_t read(uint8_t ch)
{
  uint8_t ret=0;
  switch(ch)
  {
    case _DEF_MOTOR1:

    ret=uartRead(motor_ch_tbl[ch]);
    break;
    case _DEF_MOTOR2:
    ret=uartRead(motor_ch_tbl[ch]);
    break;
  }
  return ret;
}
bool flush(uint8_t ch)
{
  bool ret = false;
switch(ch)
{
  case _DEF_MOTOR1:
    ret=uartFlush(motor_ch_tbl[ch]);
    break;
  case _DEF_MOTOR2:
    ret=uartFlush(motor_ch_tbl[ch]);
    break;
}
return ret;
}

#endif
