################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/I2C/I2C.c 

OBJS += \
./Driver/I2C/I2C.o 

C_DEPS += \
./Driver/I2C/I2C.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/I2C/%.o Driver/I2C/%.su Driver/I2C/%.cyclo: ../Driver/I2C/%.c Driver/I2C/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/STM32F407_Projects/Time_Delay_Relay_V1/Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Driver-2f-I2C

clean-Driver-2f-I2C:
	-$(RM) ./Driver/I2C/I2C.cyclo ./Driver/I2C/I2C.d ./Driver/I2C/I2C.o ./Driver/I2C/I2C.su

.PHONY: clean-Driver-2f-I2C

