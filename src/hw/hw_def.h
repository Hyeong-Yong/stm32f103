/*
 * hw_def.h
 *
 *  Created on: Dec 31, 2021
 *      Author: HYJH
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"

#define _USB_HW_USB
//#define _USE_HW_RTC
//#define _USB_HW_RESET
#define _USE_HW_CDC
#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH      4


#define _USE_HW_ADC
#define         ADC_MAX_CH	 1
#define _USE_HW_SPI
#define      HW_SPI_MAX_CH	 2

#define _USE_HW_MCP48x2
#define DAC_TYPE_MCP4822
//#define _USE_HW_FLASH

//#define _USE_HW_MOTOR
//#define      HW_MOTOR_PKT_BUF_MAX   10

#define _USE_HW_LED
#define      HW_LED_MAX_CH       1

#define _USE_HW_UART
#define      HW_UART_MAX_CH      2

#define _USE_HW_CLI
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    32


#endif /* SRC_HW_HW_DEF_H_ */
