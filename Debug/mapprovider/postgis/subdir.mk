################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../mapprovider/postgis/PostGisProvider.cc \
../mapprovider/postgis/RestrictionQueries.cc \
../mapprovider/postgis/TopologyQueries.cc 

OBJS += \
./mapprovider/postgis/PostGisProvider.o \
./mapprovider/postgis/RestrictionQueries.o \
./mapprovider/postgis/TopologyQueries.o 

CC_DEPS += \
./mapprovider/postgis/PostGisProvider.d \
./mapprovider/postgis/RestrictionQueries.d \
./mapprovider/postgis/TopologyQueries.d 


# Each subdirectory must supply rules for building sources it contributes
mapprovider/postgis/%.o: ../mapprovider/postgis/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


