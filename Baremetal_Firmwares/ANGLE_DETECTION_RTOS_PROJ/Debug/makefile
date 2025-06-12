################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RTOS/FreeRTOS/portable/MemMang/subdir.mk
-include RTOS/FreeRTOS/portable/GCC/ARM_CM4F/subdir.mk
-include RTOS/FreeRTOS/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := ANGLE_DETECTION_RTOS_PROJ
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ANGLE_DETECTION_RTOS_PROJ.elf \

MAP_FILES += \
ANGLE_DETECTION_RTOS_PROJ.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ANGLE_DETECTION_RTOS_PROJ.list \

OBJCOPY_BIN += \
ANGLE_DETECTION_RTOS_PROJ.bin \


# All Target
all: main-build

# Main-build Target
main-build: ANGLE_DETECTION_RTOS_PROJ.elf secondary-outputs

# Tool invocations
ANGLE_DETECTION_RTOS_PROJ.elf ANGLE_DETECTION_RTOS_PROJ.map: $(OBJS) $(USER_OBJS) C:\Users\kjeya\Documents\STM32F446RET6_baremetal_docs\Baremetal_Firmwares\ANGLE_DETECTION_RTOS_PROJ\STM32F446RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "ANGLE_DETECTION_RTOS_PROJ.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\kjeya\Documents\STM32F446RET6_baremetal_docs\Baremetal_Firmwares\ANGLE_DETECTION_RTOS_PROJ\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ANGLE_DETECTION_RTOS_PROJ.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ANGLE_DETECTION_RTOS_PROJ.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ANGLE_DETECTION_RTOS_PROJ.list"
	@echo 'Finished building: $@'
	@echo ' '

ANGLE_DETECTION_RTOS_PROJ.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ANGLE_DETECTION_RTOS_PROJ.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ANGLE_DETECTION_RTOS_PROJ.bin ANGLE_DETECTION_RTOS_PROJ.elf ANGLE_DETECTION_RTOS_PROJ.list ANGLE_DETECTION_RTOS_PROJ.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
