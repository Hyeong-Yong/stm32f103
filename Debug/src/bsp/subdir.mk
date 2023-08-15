################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bsp/bsp.c \
../src/bsp/stm32f1xx_hal_msp.c \
../src/bsp/stm32f1xx_it.c \
../src/bsp/syscalls.c \
../src/bsp/sysmem.c \
../src/bsp/system_stm32f1xx.c 

OBJS += \
./src/bsp/bsp.o \
./src/bsp/stm32f1xx_hal_msp.o \
./src/bsp/stm32f1xx_it.o \
./src/bsp/syscalls.o \
./src/bsp/sysmem.o \
./src/bsp/system_stm32f1xx.o 

C_DEPS += \
./src/bsp/bsp.d \
./src/bsp/stm32f1xx_hal_msp.d \
./src/bsp/stm32f1xx_it.d \
./src/bsp/syscalls.d \
./src/bsp/sysmem.d \
./src/bsp/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/bsp/%.o src/bsp/%.su src/bsp/%.cyclo: ../src/bsp/%.c src/bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSER_VECT_TAB_ADDRESS -DSTM32F103xB -c -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/ap" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/common/hw/include/spi" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/common/core" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/common" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/hw" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/bsp" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/common/hw/include" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/common/hw/include/motor" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/Drivers/CMSIS/Include" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/USB_DEVICE/App" -I"C:/Dev/STM32/stm32f1xx/stm32f103/src/lib/cube_f103/USB_DEVICE/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-bsp

clean-src-2f-bsp:
	-$(RM) ./src/bsp/bsp.cyclo ./src/bsp/bsp.d ./src/bsp/bsp.o ./src/bsp/bsp.su ./src/bsp/stm32f1xx_hal_msp.cyclo ./src/bsp/stm32f1xx_hal_msp.d ./src/bsp/stm32f1xx_hal_msp.o ./src/bsp/stm32f1xx_hal_msp.su ./src/bsp/stm32f1xx_it.cyclo ./src/bsp/stm32f1xx_it.d ./src/bsp/stm32f1xx_it.o ./src/bsp/stm32f1xx_it.su ./src/bsp/syscalls.cyclo ./src/bsp/syscalls.d ./src/bsp/syscalls.o ./src/bsp/syscalls.su ./src/bsp/sysmem.cyclo ./src/bsp/sysmem.d ./src/bsp/sysmem.o ./src/bsp/sysmem.su ./src/bsp/system_stm32f1xx.cyclo ./src/bsp/system_stm32f1xx.d ./src/bsp/system_stm32f1xx.o ./src/bsp/system_stm32f1xx.su

.PHONY: clean-src-2f-bsp

