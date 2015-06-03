################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../mapprovider/database/catchtest/PostGisProvider_test.cc 

OBJS += \
./mapprovider/database/catchtest/PostGisProvider_test.o 

CC_DEPS += \
./mapprovider/database/catchtest/PostGisProvider_test.d 


# Each subdirectory must supply rules for building sources it contributes
mapprovider/database/catchtest/%.o: ../mapprovider/database/catchtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


