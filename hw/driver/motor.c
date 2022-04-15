/*
 * motor.c
 *
 *  Created on: 2022. 1. 8.
 *      Author: HYJH
 */

#include "motor.h"
#include "cli.h"
#include "motor/motor_uart.h"

#define STX	0xFF

#ifdef _USE_HW_MOTOR

enum state
{
  STATE_HEADER=0,
  STATE_LENGTH,
  STATE_INST,
  STATE_PARAM_1,
  STATE_PARAM_2,
  STATE_PARAM_3,
  STATE_PARAM_4,
  STATE_CHECK,
};
enum index
{
  INDEX_HEADER=0,
  INDEX_LENGTH,
  INDEX_INST,
  INDEX_PARAM_1,
  INDEX_PARAM_2,
  INDEX_PARAM_3,
  INDEX_PARAM_4,
  INDEX_CHECK,
};
enum instruction
  {
    INST_SET_ANGLE =0x01,
    INST_SET_RPM = 0x02,
    INST_CHANGE_DIRECTION = 0x03,
    INST_SET_ZERO = 0x04,
    INST_GET_ANGLE = 0x05,
    INST_GET_RPM =0x06,
    INST_GET_DIRECTION = 0x07,
  };
motor_packet_t packet;
motor_t motor;
motor_status_t motor_status;
static uint32_t step = 0;

static void halfDrive(uint32_t step);
static bool checksumPacket(motor_t* p_motor);
#ifdef _USE_HW_CLI
  static void cliMotor(cli_args_t *args);
#endif

bool motorLoadDriver(motor_t* p_motor, bool (*load_func)(motor_driver_t*))
  {
    bool ret;

    ret = load_func(&p_motor->driver);
    return ret;
  }
bool motorOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;
  if (motor.driver.isInit ==false)
    {
      return false;
    }

  motor.ch=ch;
  motor.baud=baud;
  motor.isOpen=motor.driver.open(ch, baud);
  motor.pre_time=millis();
  motor.state=STATE_HEADER;

  ret = motor.isOpen;
  return ret;
}
bool motorIsOpen(motor_t *p_motor)
{
  return p_motor->isOpen;
}
bool motorClose(motor_t *p_motor)
{
  bool ret= true;

  return ret;
}


// cli 다시 검토 및 send함수 작성
// 항상 motorUart 오픈 또는 선택시 motorUart 오픈

bool motorSendInst(motor_t *p_motor, uint8_t inst, uint8_t *param);
bool motorReceivePacket(motor_t *p_motor)
{
  bool ret= false;
  uint8_t rx_data;
  uint32_t pre_time;

  if (p_motor->isOpen == false)
    {
      return ret;
    }

  pre_time=millis();
  while (p_motor->driver.available(p_motor->ch)>0)
    {
      rx_data = p_motor->driver.read(p_motor->ch);
      ret = motorProcessPKT(p_motor, rx_data);

      if (ret == true)
	{
	  break;
	}

      if (millis() - pre_time >= 50)
	{
	  break;
	}
    }
  return ret;
}
bool motorProcessPKT(motor_t *p_motor, uint8_t rx_data)
{
  bool ret =false;

  if ( millis() - p_motor->pre_time > 100)
    {
      p_motor->state = STATE_HEADER;
    }
  p_motor->pre_time= millis();

  switch (p_motor->state)
  {
    case STATE_HEADER:

      if (rx_data == STX)
	{
	  p_motor->packet_buf[INDEX_HEADER]= rx_data;
	  p_motor->state = STATE_LENGTH;
	}
      break;

    case STATE_LENGTH:
      p_motor->packet_buf[INDEX_LENGTH]= rx_data;
      p_motor->state= STATE_INST;
      break;

    case STATE_INST:
      p_motor->packet_buf[INDEX_INST]= rx_data;
      p_motor->state = STATE_PARAM_1;
      break;

    case STATE_PARAM_1:
      p_motor->packet_buf[INDEX_PARAM_1] = rx_data;
      p_motor->state= STATE_PARAM_2;
      break;

    case STATE_PARAM_2:
      p_motor->packet_buf[INDEX_PARAM_2] = rx_data;
      p_motor->state= STATE_PARAM_3;
      break;

    case STATE_PARAM_3:
      p_motor->packet_buf[INDEX_PARAM_3] = rx_data;
      p_motor->state= STATE_PARAM_4;
      break;

    case STATE_PARAM_4:
      p_motor->packet_buf[INDEX_PARAM_4] = rx_data;
      p_motor->state= STATE_CHECK;
      break;

    case STATE_CHECK:
      p_motor->packet_buf[INDEX_CHECK] = rx_data;
      if (checksumPacket(&motor) == true)
	{
	  packet.header = p_motor->packet_buf[INDEX_HEADER];
	  packet.inst   = p_motor->packet_buf[INDEX_INST];
	  packet.length = p_motor->packet_buf[INDEX_LENGTH];
	  packet.inst   = p_motor->packet_buf[INDEX_INST];
	  packet.param  = p_motor->packet_buf[INDEX_PARAM_1] << 0;
	  packet.param |= p_motor->packet_buf[INDEX_PARAM_2] << 8;
	  packet.param |= p_motor->packet_buf[INDEX_PARAM_3] << 16;
	  packet.param |= p_motor->packet_buf[INDEX_PARAM_4] << 24;
	  packet.check  = p_motor->packet_buf[INDEX_CHECK];
	  ret=true;
	  motor_status.flagGUI=true;
	  p_motor->state = STATE_HEADER;
	}
      else
	{
	  p_motor->state = STATE_HEADER;
	}
      break;
  }
  return ret;
}
bool checksumPacket(motor_t* p_motor)
{
  bool ret = false;
  uint32_t sum = 0, total = 0;
  for (int i = 0; i < INDEX_CHECK+1; i++)
    {
      sum += p_motor->packet_buf[i];
    }
  total = sum;
  total = total & 0xFF;
  total = (~total + 1) & 0xFF;
  total += sum;
  total = total & 0xFF;
  if (total == 0)
    {
      return ret = true;
    }
  return ret;
}

bool motorInit(void)
{
  bool ret= true;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  motor_status.current_angle = 0;
  motor_status.current_rpm = 5;
  motor_status.direction=true; //true :clockwise

#ifdef _USE_HW_CLI
  cliAdd("motor", cliMotor);
#endif

  motorLoadDriver(&motor, motorUartDriver);

  return ret;
}

void setRpm(motor_status_t* p_motor, uint32_t rpm)
{
  p_motor->current_rpm = rpm;
}
void Rpm(motor_status_t* p_motor)
{
  usDelay(60000000/400/p_motor->current_rpm);
}
void halfDrive(uint32_t step)
{
  switch (step){
    case 0:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN4
        break;

    case 1:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN4
        break;

    case 2:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN4
        break;

    case 3:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN4
        break;

    case 4:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN4
        break;

    case 5:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // IN4
        break;

    case 6:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // IN4
        break;

    case 7:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);   // IN2
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);   // IN3
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // IN4
        break;
    }
}

void setRotation(motor_status_t *p_motor, uint32_t angle)
{
  uint32_t ret = p_motor->current_angle;
  float rev = 400.0/360.0 * (float)angle;
  uint32_t s = (uint32_t)(rev / 8); // sequence period, 8 : total step
  uint32_t count = 0;
  for (int i = 0; i < s + 1; i++) //s+1 : one more time running at the last sequence
    {
      if (p_motor->direction == true) // for clockwise
	{
 	  ret = (p_motor->current_angle + angle) % 360;
	  for (int j = 0; j < 8; j++) //steps
	    {
	      if (count * 8 + j < rev) // stopping rotation
		{
		  if (step > 0)
		    {step--;}
		  else
		    {step=7;}

		  halfDrive(step); // Running code : moving motor, controlling speed
		  Rpm(p_motor);
		}
	    }
	}

      if (p_motor->direction == false)   // for Aniclockwise
	{
	  uint32_t numSeq = 1+angle/360;
	  if (p_motor->current_angle >= angle)
	    {ret = (p_motor->current_angle - angle) % 360;}
	  if (p_motor->current_angle < angle)
	    {ret = (360*numSeq+p_motor->current_angle-angle) % 360;}

	  for (int j = 0; j < 8; j++) //steps
	    {
	      if (count * 8 + j < rev) // stopping rotation
		{
		  if (step < 7)
		    {step++;}
		  else
		    {step=0;}

		  halfDrive(step); // Running code : moving motor, controlling speed
		  Rpm(p_motor);
		}
	    }
	}
      count++;
    }

  p_motor->current_angle = ret; //Update current angle
}
uint32_t getRpm(motor_status_t* p_motor) // current_rpm 속도
{
    return p_motor->current_rpm;
}
uint32_t getAngle(motor_status_t* p_motor) // 반환값이 현재 각도의 값이다.
{
  return p_motor->current_angle;
}
void setZeroAngle(motor_status_t* p_motor) // 현재 각도를 0으로 설정.
{
  p_motor->current_angle =0;
}
void setDirection(motor_status_t* p_motor) // 방향 전환하기 (디폴트는 시계방향)
{
  if (p_motor->direction==true)
    {
      p_motor->direction=false;
    }
  else if (p_motor->direction==false)
    {
      p_motor->direction=true;
    }
}

#ifdef _USE_HW_CLI
void getStatus(motor_status_t* p_motor) // display status (angle, speed, direction)
{
  cliPrintf("Current angle     : %d\n", p_motor->current_angle);
  cliPrintf("Current RPM       : %d\n", p_motor->current_rpm);
  if (p_motor->direction==true)
	{
	  cliPrintf("Current direction : Clockwise\n");
	}
  else
	{
	  cliPrintf("Current direction : AntiClockwise\n");
	}
}
#endif

#ifdef _USE_HW_CLI
void cliMotor(cli_args_t *args)
{
  bool ret = false;
  if (args->argc == 2 && args->isStr(0, "rotation") == true) //"motor", "rot", degree:10
    {
      uint32_t degree;
      degree = (uint32_t)args->getData(1);
      setRotation(&motor_status, degree);
      cliPrintf("Current angle is %d\n", motor_status.current_angle);
      ret= true;
    }

  if (args->argc == 2 && args->isStr(0, "speed") == true) //"motor", "rpm", 3~
    {
      uint32_t rpm;
      rpm = (uint32_t)args->getData(1);
      setRpm(&motor_status, rpm);
      cliPrintf("Current RPM is %d\n", motor_status.current_rpm);
      ret = true;
    }

  if (args->argc == 1 && args->isStr(0, "direction") == true) //"motor", "direction"
    {
      setDirection(&motor_status);
      if (motor_status.direction==true)
	{
	  cliPrintf("Clockwise\n");
	}
      else
	{
	  cliPrintf("AntiClockwise\n");
      }
      ret = true;
    }

  if (args->argc == 1 && args->isStr(0, "zero") == true) //"motor", "zero"
    {
      setZeroAngle(&motor_status);
      cliPrintf("Current angle is %d\n", motor_status.current_angle);
      ret = true;
    }

  if (args->argc == 1 && args->isStr(0, "info") == true) //"motor", "info"
    {
      getStatus(&motor_status);
      ret = true;
    }



  if (ret != true)
    {
      cliPrintf("motor rotation degree[0~360]\n");
      cliPrintf("motor speed rpm[3~]\n");
      cliPrintf("motor direction\n");
      cliPrintf("motor zero\n");
      cliPrintf("motor info\n");
    }
}
#endif

void motorGUIRun()
{
  /*INSTUCTION
0x01 : rotate angle
0x02 : set rpm
0x03 : change direction
0x04 : set zeroAngle
0x05 : get angle
0x06 : get rpm
0x07 : get direction
0x55 : open */

  if (motor.driver.available(_DEF_MOTOR1) > 0)
    {
      motorReceivePacket(&motor);
    }

  if (motor_status.flagGUI == true)
    {
      switch (packet.inst)
      {
	case INST_SET_ANGLE:
	  setRotation(&motor_status, packet.param);
	break;

	case INST_SET_RPM:
	  setRpm(&motor_status, packet.param);
	break;

	case INST_CHANGE_DIRECTION:
	  if (motor_status.direction == true)
	    {
	      motor_status.direction = false;
	    }
	  else
	    {
	      motor_status.direction = true;
	    }
	break;

	case INST_SET_ZERO:
	  motor_status.current_angle = 0;
	break;

      }
      motor_status.flagGUI = false;
    }
}

#endif
