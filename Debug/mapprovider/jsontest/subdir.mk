################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../mapprovider/jsontest/JsonTestProvider.cc 

CC_DEPS += \
./mapprovider/jsontest/JsonTestProvider.d 

OBJS += \
./mapprovider/jsontest/JsonTestProvider.o 


# Each subdirectory must supply rules for building sources it contributes
mapprovider/jsontest/%.o: ../mapprovider/jsontest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


