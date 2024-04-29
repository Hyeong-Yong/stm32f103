################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/common/core/qbuffer.c 

OBJS += \
./src/common/core/qbuffer.o 

C_DEPS += \
./src/common/core/qbuffer.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/core/%.o src/common/core/%.su src/common/core/%.cyclo: ../src/common/core/%.c src/common/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSER_VECT_TAB_ADDRESS -DSTM32F103xB -c -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/ap" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/hw/include/spi" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/core" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/hw" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/bsp" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/hw/include" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/common/hw/include/motor" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Drivers/CMSIS/Include" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/USB_DEVICE/App" -I"C:/Dev/STM32/STM32F1xx/stm32f103/src/lib/cube_f103/USB_DEVICE/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-common-2f-core

clean-src-2f-common-2f-core:
	-$(RM) ./src/common/core/qbuffer.cyclo ./src/common/core/qbuffer.d ./src/common/core/qbuffer.o ./src/common/core/qbuffer.su

.PHONY: clean-src-2f-common-2f-core

