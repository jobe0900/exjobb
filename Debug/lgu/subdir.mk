################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../lgu/LineGraphUtility.cc \
../lgu/Logging.cc 

OBJS += \
./lgu/LineGraphUtility.o \
./lgu/Logging.o 

CC_DEPS += \
./lgu/LineGraphUtility.d \
./lgu/Logging.d 


# Each subdirectory must supply rules for building sources it contributes
lgu/%.o: ../lgu/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

