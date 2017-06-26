################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/middleware/fat/diskio.c \
../src/middleware/fat/fatfs.c \
../src/middleware/fat/fatfs_platform.c \
../src/middleware/fat/ff.c \
../src/middleware/fat/ff_gen_drv.c \
../src/middleware/fat/qspi_diskio.c \
../src/middleware/fat/sd_diskio.c \
../src/middleware/fat/sdram_diskio.c \
../src/middleware/fat/syscall.c 

OBJS += \
./src/middleware/fat/diskio.o \
./src/middleware/fat/fatfs.o \
./src/middleware/fat/fatfs_platform.o \
./src/middleware/fat/ff.o \
./src/middleware/fat/ff_gen_drv.o \
./src/middleware/fat/qspi_diskio.o \
./src/middleware/fat/sd_diskio.o \
./src/middleware/fat/sdram_diskio.o \
./src/middleware/fat/syscall.o 

C_DEPS += \
./src/middleware/fat/diskio.d \
./src/middleware/fat/fatfs.d \
./src/middleware/fat/fatfs_platform.d \
./src/middleware/fat/ff.d \
./src/middleware/fat/ff_gen_drv.d \
./src/middleware/fat/qspi_diskio.d \
./src/middleware/fat/sd_diskio.d \
./src/middleware/fat/sdram_diskio.d \
./src/middleware/fat/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
src/middleware/fat/%.o: ../src/middleware/fat/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F469xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


