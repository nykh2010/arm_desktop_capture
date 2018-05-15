################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/device.c \
../source/file.c \
../source/log.c \
../source/main.c \
../source/source.c 

OBJS += \
./source/device.o \
./source/file.o \
./source/log.o \
./source/main.o \
./source/source.o 

C_DEPS += \
./source/device.d \
./source/file.d \
./source/log.d \
./source/main.d \
./source/source.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	arm-linux-gnueabihf-g++ -I"/home/nykh/workspace/desktop_capture/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


