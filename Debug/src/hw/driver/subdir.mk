################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hw/driver/adc.c \
../src/hw/driver/cdc.c \
../src/hw/driver/cli.c \
../src/hw/driver/flash.c \
../src/hw/driver/galvano.c \
../src/hw/driver/gpio.c \
../src/hw/driver/led.c \
../src/hw/driver/motor.c \
../src/hw/driver/reset.c \
../src/hw/driver/rtc.c \
../src/hw/driver/spi.c \
../src/hw/driver/uart.c \
../src/hw/driver/usb.c 

OBJS += \
./src/hw/driver/adc.o \
./src/hw/driver/cdc.o \
./src/hw/driver/cli.o \
./src/hw/driver/flash.o \
./src/hw/driver/galvano.o \
./src/hw/driver/gpio.o \
./src/hw/driver/led.o \
./src/hw/driver/motor.o \
./src/hw/driver/reset.o \
./src/hw/driver/rtc.o \
./src/hw/driver/spi.o \
./src/hw/driver/uart.o \
./src/hw/driver/usb.o 

C_DEPS += \
./src/hw/driver/adc.d \
./src/hw/driver/cdc.d \
./src/hw/driver/cli.d \
./src/hw/driver/flash.d \
./src/hw/driver/galvano.d \
./src/hw/driver/gpio.d \
./src/hw/driver/led.d \
./src/hw/driver/motor.d \
./src/hw/driver/reset.d \
./src/hw/driver/rtc.d \
./src/hw/driver/spi.d \
./src/hw/driver/uart.d \
./src/hw/driver/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/hw/driver/%.o src/hw/driver/%.su src/hw/driver/%.cyclo: ../src/hw/driver/%.c src/hw/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSER_VECT_TAB_ADDRESS -DSTM32F103xB -c -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/ap" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/hw/include/spi" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/core" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/hw" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/bsp" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/hw/include" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/hw/include/motor" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Drivers/CMSIS/Include" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/USB_DEVICE/App" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/USB_DEVICE/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-hw-2f-driver

clean-src-2f-hw-2f-driver:
	-$(RM) ./src/hw/driver/adc.cyclo ./src/hw/driver/adc.d ./src/hw/driver/adc.o ./src/hw/driver/adc.su ./src/hw/driver/cdc.cyclo ./src/hw/driver/cdc.d ./src/hw/driver/cdc.o ./src/hw/driver/cdc.su ./src/hw/driver/cli.cyclo ./src/hw/driver/cli.d ./src/hw/driver/cli.o ./src/hw/driver/cli.su ./src/hw/driver/flash.cyclo ./src/hw/driver/flash.d ./src/hw/driver/flash.o ./src/hw/driver/flash.su ./src/hw/driver/galvano.cyclo ./src/hw/driver/galvano.d ./src/hw/driver/galvano.o ./src/hw/driver/galvano.su ./src/hw/driver/gpio.cyclo ./src/hw/driver/gpio.d ./src/hw/driver/gpio.o ./src/hw/driver/gpio.su ./src/hw/driver/led.cyclo ./src/hw/driver/led.d ./src/hw/driver/led.o ./src/hw/driver/led.su ./src/hw/driver/motor.cyclo ./src/hw/driver/motor.d ./src/hw/driver/motor.o ./src/hw/driver/motor.su ./src/hw/driver/reset.cyclo ./src/hw/driver/reset.d ./src/hw/driver/reset.o ./src/hw/driver/reset.su ./src/hw/driver/rtc.cyclo ./src/hw/driver/rtc.d ./src/hw/driver/rtc.o ./src/hw/driver/rtc.su ./src/hw/driver/spi.cyclo ./src/hw/driver/spi.d ./src/hw/driver/spi.o ./src/hw/driver/spi.su ./src/hw/driver/uart.cyclo ./src/hw/driver/uart.d ./src/hw/driver/uart.o ./src/hw/driver/uart.su ./src/hw/driver/usb.cyclo ./src/hw/driver/usb.d ./src/hw/driver/usb.o ./src/hw/driver/usb.su

.PHONY: clean-src-2f-hw-2f-driver

