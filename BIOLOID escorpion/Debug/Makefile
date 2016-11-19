################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../adc.c \
../BioloidCControl.c \
../button.c \
../buzzer.c \
../clock.c \
../dxl_hal.c \
../dynamixel.c \
../led.c \
../motion.c \
../pose.c \
../serial.c \
../walk.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
adc.o \
BioloidCControl.o \
button.o \
buzzer.o \
clock.o \
dxl_hal.o \
dynamixel.o \
led.o \
motion.o \
pose.o \
serial.o \
walk.o


OBJS_AS_ARGS +=  \
adc.o \
BioloidCControl.o \
button.o \
buzzer.o \
clock.o \
dxl_hal.o \
dynamixel.o \
led.o \
motion.o \
pose.o \
serial.o \
walk.o


C_DEPS +=  \
adc.d \
BioloidCControl.d \
button.d \
buzzer.d \
clock.d \
dxl_hal.d \
dynamixel.d \
led.d \
motion.d \
pose.d \
serial.d \
walk.d


C_DEPS_AS_ARGS +=  \
adc.d \
BioloidCControl.d \
button.d \
buzzer.d \
clock.d \
dxl_hal.d \
dynamixel.d \
led.d \
motion.d \
pose.d \
serial.d \
walk.d


OUTPUT_FILE_PATH +=BioloidCControl.elf

OUTPUT_FILE_PATH_AS_ARGS +=BioloidCControl.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR32/GNU C Compiler

























./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL  -Os -fpack-struct -fshort-enums -Wall -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -mmcu=atmega2561  -o"$@" "$<" 
	@echo Finished building: $<



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU C Linker
	$(QUOTE)C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="BioloidCControl.map" -Wl,-lm  -Wl,--defsym=__stack=0xfe00  -mmcu=atmega2561  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "BioloidCControl.elf" "BioloidCControl.hex"
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "BioloidCControl.elf" "BioloidCControl.eep" || exit 0
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-objdump.exe" -h -S "BioloidCControl.elf" > "BioloidCControl.lss"
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-size.exe" -C --mcu=atmega2561  "BioloidCControl.elf"
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) 
	rm -rf "BioloidCControl.hex" "BioloidCControl.lss" "BioloidCControl.eep" "BioloidCControl.map"
	