################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/arial10.c \
../src/arial10b.c \
../src/arial18.c \
../src/arial18b.c \
../src/arial24.c \
../src/arial24b.c \
../src/main.c 

OBJS += \
./src/arial10.o \
./src/arial10b.o \
./src/arial18.o \
./src/arial18b.o \
./src/arial24.o \
./src/arial24b.o \
./src/main.o 

C_DEPS += \
./src/arial10.d \
./src/arial10b.d \
./src/arial18.d \
./src/arial18b.d \
./src/arial24.d \
./src/arial24b.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F469xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


