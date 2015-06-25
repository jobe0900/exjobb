################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../osm/catchtest/OsmAccess_test.cc \
../osm/catchtest/OsmBarrier_test.cc \
../osm/catchtest/OsmHighway_test.cc \
../osm/catchtest/OsmTurningRestriction_test.cc \
../osm/catchtest/OsmVehicle_test.cc 

CC_DEPS += \
./osm/catchtest/OsmAccess_test.d \
./osm/catchtest/OsmBarrier_test.d \
./osm/catchtest/OsmHighway_test.d \
./osm/catchtest/OsmTurningRestriction_test.d \
./osm/catchtest/OsmVehicle_test.d 

OBJS += \
./osm/catchtest/OsmAccess_test.o \
./osm/catchtest/OsmBarrier_test.o \
./osm/catchtest/OsmHighway_test.o \
./osm/catchtest/OsmTurningRestriction_test.o \
./osm/catchtest/OsmVehicle_test.o 


# Each subdirectory must supply rules for building sources it contributes
osm/catchtest/%.o: ../osm/catchtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


