################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include util/subdir.mk
-include osm/catchtest/subdir.mk
-include osm/subdir.mk
-include mapprovider/postgis/catchtest/subdir.mk
-include mapprovider/postgis/subdir.mk
-include mapprovider/jsontest/catchtest/subdir.mk
-include mapprovider/jsontest/subdir.mk
-include mapprovider/catchtest/subdir.mk
-include lgu/catchtest/subdir.mk
-include lgu/subdir.mk
-include graph/catchtest/subdir.mk
-include graph/subdir.mk
-include config/catchtest/subdir.mk
-include config/subdir.mk
-include catchtest/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: LineGraphUtility

# Tool invocations
LineGraphUtility: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "LineGraphUtility" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) LineGraphUtility
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
