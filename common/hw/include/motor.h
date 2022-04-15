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

#define MOTOR_PKT_BUF_MAX        HW_MOTOR_PKT_BUF_MAX



typedef struct
{
  uint8_t header;
  uint8_t length;
  uint8_t inst;
  uint8_t check;
  uint32_t param;
} motor_packet_t;

enum
{
  MOTOR_INST_STATUS = 0x01,
  MOTOR_INST_READ = 0x02,
  MOTOR_INST_WRITE = 0x03,
};


typedef struct _motor_driver_t
{
  bool isInit;
  bool isOpen;

  bool (*open)(uint8_t ch, uint32_t baud);
  bool (*close)(uint8_t ch);
  uint32_t (*available)(uint8_t ch);
  uint32_t (*write)(uint8_t ch, uint8_t* p_data, uint32_t length);
  uint8_t (*read)(uint8_t ch);
  bool (*flush)(uint8_t ch);
} motor_driver_t;

typedef struct _motor_t
{
  motor_driver_t driver;

  bool 		 isOpen;
  uint8_t	 ch;
  uint32_t       baud;
  uint32_t 	 pre_time;
  uint32_t       state;

  motor_packet_t packet;
  uint8_t        packet_buf[MOTOR_PKT_BUF_MAX];
} motor_t;


typedef struct _motor_status_t
{
  uint32_t       current_rpm;
  uint32_t       current_angle;
  bool 	         direction;
  bool		 flagGUI;
}motor_status_t;

bool motorLoadDriver(motor_t* p_motor, bool (*load_func)(motor_driver_t*));

bool motorOpen(uint8_t ch, uint32_t baud);
bool motorIsOpen(motor_t *p_motor);
bool motorClose(motor_t *p_motor);




bool motorSendInst(motor_t *p_motor, uint8_t inst, uint8_t *param);
bool motorReceivePacket(motor_t *p_motor);
bool motorProcessPKT(motor_t *p_motor, uint8_t rx_data);



bool motorInit(void);



void setRpm(motor_status_t *p_motor, uint32_t rpm);
void setRotation(motor_status_t *p_motor, uint32_t angle); //파라미터 값만큼 회전하며, 반환값은 현재각도+이동한 값이다.
uint32_t getRpm(motor_status_t *p_motor); // current_rpm 속도
uint32_t getAngle(motor_status_t *p_motor); // 반환값이 현재 각도의 값이다.
void zeroAngle(motor_status_t *p_motor); // 현재 각도를 0으로 설정.
void setDirection(motor_status_t *p_motor); // 방향 전환하기(디폴트는 시계방향)
void getStatus(motor_status_t *p_motor); // display status (angle, speed, direction)


void motorGUIRun();

#endif

#endif /* SRC_COMMON_HW_INCLUDE_MOTOR_H_ */
