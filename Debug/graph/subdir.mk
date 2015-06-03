################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../graph/Edge.cc \
../graph/EdgeRestrictions.cc \
../graph/Graph.cc \
../graph/Topology.cc \
../graph/Vertex.cc 

OBJS += \
./graph/Edge.o \
./graph/EdgeRestrictions.o \
./graph/Graph.o \
./graph/Topology.o \
./graph/Vertex.o 

CC_DEPS += \
./graph/Edge.d \
./graph/EdgeRestrictions.d \
./graph/Graph.d \
./graph/Topology.d \
./graph/Vertex.d 


# Each subdirectory must supply rules for building sources it contributes
graph/%.o: ../graph/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -std=c++11 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


