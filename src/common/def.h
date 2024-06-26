/*
 * def.h
 *
 *  Created on: Dec 31, 2021
 *      Author: HYJH
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define _DEF_LED1    0
#define _DEF_LED2    1
#define _DEF_LED3    2
#define _DEF_LED4    3

#define _DEF_ADC1	 0

#define _DEF_UART1   0  // USB_CDC communication
#define _DEF_UART2   1  // UART
#define _DEF_UART3   2
#define _DEF_UART4   3

#define _DEF_MOTOR1 0
#define _DEF_MOTOR2 1

#define _DEF_INPUT		0
#define _DEF_INPUT_PULLUP	1
#define _DEF_INPUT_PULLDOWN	2
#define _DEF_OUTPUT		3
#define _DEF_OUTPUT_PULLUP	4
#define _DEF_OUTPUT_PULLDOWN	5

#define _DEF_GPIO1	0
#define _DEF_GPIO2	1
#define _DEF_GPIO3	2  //GPIO DAC_CS (DAC_MSP4822)
#define _DEF_GPIO4	3  //GPIO DAC_LDAC (DAC_MSP4822)

#define _DEF_SPI1 		0
#define _DEF_SPI2		1//DAC_MSP4822

#define _DEF_RESET		0
#define _DEF_SET		1

#endif /* SRC_COMMON_DEF_H_ */
