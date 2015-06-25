################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../mapprovider/postgis/catchtest/PostGisProvider_test.cc 

CC_DEPS += \
./mapprovider/postgis/catchtest/PostGisProvider_test.d 

OBJS += \
./mapprovider/postgis/catchtest/PostGisProvider_test.o 


# Each subdirectory must supply rules for building sources it contributes
mapprovider/postgis/catchtest/%.o: ../mapprovider/postgis/catchtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


