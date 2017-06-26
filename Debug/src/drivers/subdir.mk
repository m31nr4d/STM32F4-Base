################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/_write.c \
../src/drivers/bsp.c \
../src/drivers/cs43l22.c \
../src/drivers/eeprom.c \
../src/drivers/font12.c \
../src/drivers/font16.c \
../src/drivers/font20.c \
../src/drivers/font24.c \
../src/drivers/font8.c \
../src/drivers/ft6x06.c \
../src/drivers/lcd.c \
../src/drivers/otm8009a.c \
../src/drivers/qspi.c \
../src/drivers/sd.c \
../src/drivers/sdram.c \
../src/drivers/stm32f4xx_hal_msp.c \
../src/drivers/stm32f4xx_it.c \
../src/drivers/system_stm32f4xx.c \
../src/drivers/ts.c 

OBJS += \
./src/drivers/_write.o \
./src/drivers/bsp.o \
./src/drivers/cs43l22.o \
./src/drivers/eeprom.o \
./src/drivers/font12.o \
./src/drivers/font16.o \
./src/drivers/font20.o \
./src/drivers/font24.o \
./src/drivers/font8.o \
./src/drivers/ft6x06.o \
./src/drivers/lcd.o \
./src/drivers/otm8009a.o \
./src/drivers/qspi.o \
./src/drivers/sd.o \
./src/drivers/sdram.o \
./src/drivers/stm32f4xx_hal_msp.o \
./src/drivers/stm32f4xx_it.o \
./src/drivers/system_stm32f4xx.o \
./src/drivers/ts.o 

C_DEPS += \
./src/drivers/_write.d \
./src/drivers/bsp.d \
./src/drivers/cs43l22.d \
./src/drivers/eeprom.d \
./src/drivers/font12.d \
./src/drivers/font16.d \
./src/drivers/font20.d \
./src/drivers/font24.d \
./src/drivers/font8.d \
./src/drivers/ft6x06.d \
./src/drivers/lcd.d \
./src/drivers/otm8009a.d \
./src/drivers/qspi.d \
./src/drivers/sd.d \
./src/drivers/sdram.d \
./src/drivers/stm32f4xx_hal_msp.d \
./src/drivers/stm32f4xx_it.d \
./src/drivers/system_stm32f4xx.d \
./src/drivers/ts.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/%.o: ../src/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F469xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


