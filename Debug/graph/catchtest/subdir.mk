################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../graph/catchtest/EdgeCost_test.cc \
../graph/catchtest/GraphBuilder_test.cc \
../graph/catchtest/RestrictionsAndCosts_test.cc \
../graph/catchtest/Topology_test.cc \
../graph/catchtest/TurnCostCalculator_test.cc 

CC_DEPS += \
./graph/catchtest/EdgeCost_test.d \
./graph/catchtest/GraphBuilder_test.d \
./graph/catchtest/RestrictionsAndCosts_test.d \
./graph/catchtest/Topology_test.d \
./graph/catchtest/TurnCostCalculator_test.d 

OBJS += \
./graph/catchtest/EdgeCost_test.o \
./graph/catchtest/GraphBuilder_test.o \
./graph/catchtest/RestrictionsAndCosts_test.o \
./graph/catchtest/Topology_test.o \
./graph/catchtest/TurnCostCalculator_test.o 


# Each subdirectory must supply rules for building sources it contributes
graph/catchtest/%.o: ../graph/catchtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


