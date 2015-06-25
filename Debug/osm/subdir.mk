################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../osm/OsmAccess.cc \
../osm/OsmBarrier.cc \
../osm/OsmHighway.cc \
../osm/OsmId.cc \
../osm/OsmTurningRestriction.cc \
../osm/OsmVehicle.cc 

CC_DEPS += \
./osm/OsmAccess.d \
./osm/OsmBarrier.d \
./osm/OsmHighway.d \
./osm/OsmId.d \
./osm/OsmTurningRestriction.d \
./osm/OsmVehicle.d 

OBJS += \
./osm/OsmAccess.o \
./osm/OsmBarrier.o \
./osm/OsmHighway.o \
./osm/OsmId.o \
./osm/OsmTurningRestriction.o \
./osm/OsmVehicle.o 


# Each subdirectory must supply rules for building sources it contributes
osm/%.o: ../osm/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


