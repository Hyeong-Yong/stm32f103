/*
 * motor_uart.h
 *
 *  Created on: 2022. 4. 11.
 *      Author: HYJH
 */

#ifndef SRC_COMMON_HW_INCLUDE_MOTOR_MOTOR_UART_H_
#define SRC_COMMON_HW_INCLUDE_MOTOR_MOTOR_UART_H_

#include "hw_def.h"

#ifdef _USE_HW_MOTOR

#include "motor.h"



bool motorUartDriver(motor_driver_t* p_driver);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_MOTOR_MOTOR_UART_H_ */
