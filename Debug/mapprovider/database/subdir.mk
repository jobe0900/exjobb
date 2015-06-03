################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../mapprovider/database/PostGisProvider.cc 

OBJS += \
./mapprovider/database/PostGisProvider.o 

CC_DEPS += \
./mapprovider/database/PostGisProvider.d 


# Each subdirectory must supply rules for building sources it contributes
mapprovider/database/%.o: ../mapprovider/database/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


