/*
 * gpio.c
 *
 *  Created on: 2022. 4. 18.
 *      Author: HYJH
 */


#include "gpio.h"
#include "cli.h"

#ifdef _USE_HW_GPIO

typedef struct _gpio_tbl_t
{
  GPIO_TypeDef* 	port;
  uint32_t 		pin;
  uint8_t 		mode;
  uint8_t		speed;
  GPIO_PinState 	on_state;
  GPIO_PinState 	off_state;
  bool			init_value;
} gpio_tbl_t;

gpio_tbl_t gpio_tbl[GPIO_MAX_CH] =
    {
	{GPIOA, GPIO_PIN_6, _DEF_OUTPUT_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_HIGH}, // 0.DAC CS (_DEF_DAC_CS)
	{GPIOB, GPIO_PIN_6, _DEF_OUTPUT_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_PIN_SET, GPIO_PIN_RESET, _DEF_HIGH}, // 1.DAC LDAC (_DEF_DAC_LDAC)
    };

#ifdef _USE_HW_CLI
static void cliGpio(cli_args_t* args);
#endif



bool gpioInit()
{
  bool ret =true;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  for (int i =0; i<GPIO_MAX_CH; i++)
    {
      gpioPinMode(i, gpio_tbl[i].mode);
      gpioPinWrite(i, gpio_tbl[i].init_value);
    }

#ifdef _USE_HW_CLI
  cliAdd("gpio", cliGpio);
#endif


  return ret;
}


bool gpioPinMode(uint8_t ch, uint8_t mode)
{
bool ret= true;

GPIO_InitTypeDef GPIO_InitStruct = {0};

switch (mode)
{
  case _DEF_INPUT:
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    break;
  case _DEF_INPUT_PULLUP:
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    break;
  case _DEF_INPUT_PULLDOWN:
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    break;
  case _DEF_OUTPUT:
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    break;
  case _DEF_OUTPUT_PULLUP:

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    break;
  case _DEF_OUTPUT_PULLDOWN:
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    break;

}

GPIO_InitStruct.Pin = gpio_tbl[ch].pin;
GPIO_InitStruct.Pin = gpio_tbl[ch].speed;
HAL_GPIO_Init(gpio_tbl[ch].port, &GPIO_InitStruct);

return ret;
}

void gpioPinWrite(uint8_t ch, bool value)
{
  if (ch >= GPIO_MAX_CH)
    {
      return;
    }

  if (value == true)
    {
      HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
    }
  else
    {
      HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
    }
}

bool gpioPinRead(uint8_t ch)
{
bool ret =false;

if (ch >= GPIO_MAX_CH)
  {
    return ret;
  }

if (HAL_GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin)== gpio_tbl[ch].on_state)
  {
    ret= true;
  }

return ret;
}
void gpioPinToggle(uint8_t ch)
{
  if (ch >= GPIO_MAX_CH)
    {
      return;
    }
  HAL_GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);

}

void gpioPinLatch(uint8_t ch)
{
  if (ch >=GPIO_MAX_CH)
    {
      return;
    }
  HAL_GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);
  HAL_GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);
}


#ifdef _USE_HW_CLI
void cliGpio(cli_args_t* args)
{
  bool ret =false;

  if(args->argc ==1 && args-> isStr(0, "show") == true)
    {
      while(cliKeepLoop())
	{
	  for (int i=0; i<GPIO_MAX_CH; i++)
	    {
	      cliPrintf("%d", gpioPinRead(i));
	    }
	  cliPrintf("\n");
	  delay(100);
	}
      ret = true;
    }

  if (args->argc == 2 && args->isStr (0, "read") == true)
    {
      uint8_t ch;

      ch = (uint8_t)args->getData(1);
      while (cliKeepLoop())
	{
	  cliPrintf("gpio read %d : %d\n", ch, gpioPinRead(ch));
	  delay(100);
	}
      ret =true;
    }

  if (args->argc == 3 && args->isStr (0, "write") == true)
    {
      uint8_t ch;
      uint8_t value;
      ch = (uint8_t)args->getData(1);
      value = (uint8_t)args->getData(2);
      gpioPinWrite(ch, value);
      cliPrintf("gpio write %d : %d\n", ch, value);
      ret =true;
    }


  if (ret !=true)
    {
      cliPrintf("gpio show\n");
      cliPrintf("gpio read ch[0~%d]\n", GPIO_MAX_CH-1);
      cliPrintf("gpio write ch[0~%d] 0:1\n", GPIO_MAX_CH-1);
    }
}


#endif


#endif
