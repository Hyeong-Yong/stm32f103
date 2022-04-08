/*
 * motor.h
 *
 *  Created on: 2022. 1. 8.
 *      Author: HYJH
 */

#ifndef SRC_COMMON_HW_INCLUDE_MOTOR_H_
#define SRC_COMMON_HW_INCLUDE_MOTOR_H_

#include "hw_def.h"

#ifdef _USE_HW_MOTOR

bool motorInit(void);
static void halfDrive(uint32_t step);
void setRpm(uint32_t rpm);
void setRotation(uint32_t angle); //파라미터 값만큼 회전하며, 반환값은 현재각도+이동한 값이다.
uint32_t getRpm(); // current_rpm 속도
uint32_t getAngle(); // 반환값이 현재 각도의 값이다.
void zeroAngle(); // 현재 각도를 0으로 설정.
void setDirection(); // 방향 전환하기(디폴트는 시계방향)
void getStatus(); // display status (angle, speed, direction)



// set_pos_init
// zero_position
// current_to_zero_position is set at zero position
// state_display (angle)


#endif

#endif /* SRC_COMMON_HW_INCLUDE_MOTOR_H_ */
