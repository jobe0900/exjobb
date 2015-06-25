################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../mapprovider/catchtest/MapProvider_test.cc 

CC_DEPS += \
./mapprovider/catchtest/MapProvider_test.d 

OBJS += \
./mapprovider/catchtest/MapProvider_test.o 


# Each subdirectory must supply rules for building sources it contributes
mapprovider/catchtest/%.o: ../mapprovider/catchtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


