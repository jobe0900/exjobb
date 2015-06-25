################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../config/Configuration.cc \
../config/ConfigurationReader.cc 

CC_DEPS += \
./config/Configuration.d \
./config/ConfigurationReader.d 

OBJS += \
./config/Configuration.o \
./config/ConfigurationReader.o 


# Each subdirectory must supply rules for building sources it contributes
config/%.o: ../config/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


