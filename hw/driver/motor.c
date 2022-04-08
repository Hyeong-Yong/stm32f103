/*
 * motor.c
 *
 *  Created on: 2022. 1. 8.
 *      Author: HYJH
 */

#include "motor.h"
#include "cli.h"

typedef struct
{
  uint32_t current_rpm;
  uint32_t current_angle;
  bool 	   direction;
} motor_status_t;

motor_status_t motor_status;
static uint32_t step = 0;

#ifdef _USE_HW_CLI
  static void cliMotor(cli_args_t *args);
#endif

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

  motor_status.current_angle=0;
  motor_status.current_rpm=1;
  motor_status.direction=true; //true : clockwise

#ifdef _USE_HW_CLI
  cliAdd("motor", cliMotor);
#endif


  return ret;
}

void setRpm(uint32_t speed)
{
 uint32_t pre_time;
 pre_time =millis();

 while( (millis()-pre_time) < speed)
   {}
}

static void halfDrive(uint32_t step)
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

void setRotation(uint32_t angle)
{
  uint32_t ret = motor_status.current_angle;
  float rev = 400.0 / 360.0 * angle;
  uint32_t s = (uint32_t)(rev / 8); // sequence period, 8 : total step
  uint32_t count = 0;
  for (int i = 0; i < s + 1; i++) //s+1 : one more time running at the last sequence
    {
      if (motor_status.direction == true) // for clockwise
	{
	  ret = (motor_status.current_angle + angle) % 360;
	  for (int j = 0; j < 8; j++) //steps
	    {
	      if (count * 8 + j < rev) // stopping rotation
		{
		  if (step > 0)
		    {step--;}
		  else
		    {step = 8;}

		  halfDrive (step); // Running code : moving motor, controlling speed
		  setRpm (4);
		}
	    }
	}

      if (motor_status.direction == false)   // for Aniclockwise
	{
	  uint32_t numSeq = 1+angle/360;
	  if (motor_status.current_angle >= angle)
	    {ret = (motor_status.current_angle - angle) % 360;}
	  if (motor_status.current_angle < angle)
	    {ret = (360*numSeq+motor_status.current_angle-angle) % 360;}

	  for (int j = 0; j < 8; j++) //steps
	    {
	      if (count * 8 + j < rev) // stopping rotation
		{
		  if (step < 8)
		    {step++;}
		  else
		    {step = 0;}

		  halfDrive (j); // Running code : moving motor, controlling speed
		  setRpm (4);
		}
	    }
	}
      count++;
    }

  motor_status.current_angle = ret; //Update current angle
}

uint32_t getRpm() // current_rpm 속도
{
    return motor_status.current_rpm;
}
uint32_t getAngle() // 반환값이 현재 각도의 값이다.
{
  return motor_status.current_angle;
}
void setZeroAngle() // 현재 각도를 0으로 설정.
{
  motor_status.current_angle =0;
}
void setDirection() // 방향 전환하기 (디폴트는 시계방향)
{
  if (motor_status.direction==true)
    {
      motor_status.direction=false;
    }
  else if (motor_status.direction==false)
    {
      motor_status.direction=true;
    }
}

void getStatus() // display status (angle, speed, direction)
{
  cliPrintf("Current angle     : %d\n", motor_status.current_angle);
  cliPrintf("Current RPM       : %d\n", motor_status.current_rpm);
  if (motor_status.direction==true)
	{
	  cliPrintf("Current direction : Clockwise\n");
	}
  else
	{
	  cliPrintf("Current direction : AntiClockwise\n");
	}
}




void cliMotor(cli_args_t *args)
{
  bool ret = false;
  uint32_t degree;
  if (args->argc == 2 && args->isStr(0, "rotation") == true) //"motor", "rot", degree:10
    {
      degree = (uint32_t)args->getData(1);
      setRotation(degree);
      cliPrintf("Current angle is %d\n", motor_status.current_angle);
      ret= true;
    }

  if (args->argc == 1 && args->isStr(0, "direction") == true) //"motor", "direction"
    {
      setDirection();
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

  if (args->argc == 1 && args->isStr(0, "zero") == true) //"motor", "direction"
    {
      setZeroAngle();
      cliPrintf("Current angle is %d\n", motor_status.current_angle);
      ret = true;
    }

  if (args->argc == 1 && args->isStr(0, "info") == true) //"motor", "direction"
    {
      getStatus();
      ret = true;
    }

  if (ret != true)
    {
      cliPrintf("motor rotation degree[0~360]\n");
      cliPrintf("motor direction\n");
      cliPrintf("motor zero\n");
      cliPrintf("motor info\n");
    }
}
