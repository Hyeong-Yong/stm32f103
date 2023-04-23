/*
 * gpio.h
 *
 *  Created on: 2022. 4. 18.
 *      Author: HYJH
 */


#include "hw_def.h"

#ifdef _USE_HW_GPIO

#define GPIO_MAX_CH	HW_GPIO_MAX_CH



bool gpioInit();
bool gpioPinMode(uint8_t ch, uint8_t mode);
void gpioPinWrite(uint8_t ch, bool value);
bool gpioPinRead(uint8_t ch);
void gpioPinToggle(uint8_t ch);
void gpioPinLatch(uint8_t ch);




#endif
