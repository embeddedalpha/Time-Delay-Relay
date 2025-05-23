################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/Independent_WatchDog/Independent_WatchDog.c 

OBJS += \
./Driver/Independent_WatchDog/Independent_WatchDog.o 

C_DEPS += \
./Driver/Independent_WatchDog/Independent_WatchDog.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/Independent_WatchDog/%.o Driver/Independent_WatchDog/%.su Driver/Independent_WatchDog/%.cyclo: ../Driver/Independent_WatchDog/%.c Driver/Independent_WatchDog/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/STM32F407_Projects/Time_Delay_Relay_V1/Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Driver-2f-Independent_WatchDog

clean-Driver-2f-Independent_WatchDog:
	-$(RM) ./Driver/Independent_WatchDog/Independent_WatchDog.cyclo ./Driver/Independent_WatchDog/Independent_WatchDog.d ./Driver/Independent_WatchDog/Independent_WatchDog.o ./Driver/Independent_WatchDog/Independent_WatchDog.su

.PHONY: clean-Driver-2f-Independent_WatchDog

