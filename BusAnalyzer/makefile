#
# Makefile for STM32F103 BusAnalyzer Application
# Author: Alex
#

#Device specific setting
#DEVICE_TYPE = STM32F10X_LD #Low density (16kb-32kb Flash)
DEVICE_TYPE = STM32F10X_MD #Medium density (64kb-128kb Flash)
#DEVICE_TYPE = STM32F10X_HD #High density (256kb-512kb Flash)

#Name of the application
PROGRAM_NAME = BusAnalyzer

#Build directory
BUILD_DIR = build

CUSTOM_SOURCE_FOLDER = CustomSource
ARM_SOURCE_FOLDER = ARMSource/CMSIS/Core/CM3
STM32_SOURCE_FOLDER = STM32Source/STM32F10x_StdPeriph_Driver

#C source files from application
C_SOURCES = main.c
C_SOURCES += can.c
C_SOURCES += lin.c
C_SOURCES += usart.c
C_SOURCES += time.c
C_SOURCES += communication.c

#C source files from ARM (CMSIS)
C_SOURCES += core_cm3.c
C_SOURCES += system_stm32f10x.c

#C source files from STM
C_SOURCES += stm32f10x_gpio.c
C_SOURCES += stm32f10x_rcc.c
C_SOURCES += stm32f10x_usart.c
C_SOURCES += stm32f10x_can.c
C_SOURCES += stm32f10x_dma.c
C_SOURCES += misc.c

#Assembler source files from ARM (CMSIS)
A_SOURCES = startup_stm32f10x_md.s

#debugging options
DEBUG = -g

TARGET_ARCH = -mcpu=cortex-m3 -mthumb

#Program names
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
AS      = arm-none-eabi-as
CP      = arm-none-eabi-objcopy
OD	= arm-none-eabi-objdump
SIZE 	= arm-none-eabi-size
NM      = arm-none-eabi-nm

#Warning options
WARN_OPTS = -Wall -Wextra # -Werror

#Flags for building application
CFLAGS  = -I./ -I$(CUSTOM_SOURCE_FOLDER)/ -I$(ARM_SOURCE_FOLDER)/ -I$(STM32_SOURCE_FOLDER)/inc
CFLAGS += -c -fno-common -O0 -g3 $(DEBUG) -D$(DEVICE_TYPE) $(WARN_OPTS)
AFLAGS  = -mapcs-32
LFLAGS  = -Wl,--gc-sections,-Map=$(BUILD_DIR)/$(PROGRAM_NAME).map,-Tstm32.ld 
CPFLAGS = -Obinary
ODFLAGS	= -S

C_OBJS = $(C_SOURCES:%.c=$(BUILD_DIR)/%.o)
A_OBJS = $(A_SOURCES:%.s=$(BUILD_DIR)/%.o)
VPATH = $(CUSTOM_SOURCE_FOLDER)/:$(ARM_SOURCE_FOLDER)/:$(ARM_SOURCE_FOLDER)/startup/gcc/:$(STM32_SOURCE_FOLDER)/src

all:$(PROGRAM_NAME).elf
	@echo "Copying  " $(PROGRAM_NAME).elf " to" $(PROGRAM_NAME).bin
	@$(CP) $(CPFLAGS) $(BUILD_DIR)/$(PROGRAM_NAME).elf $(BUILD_DIR)/$(PROGRAM_NAME).bin
	@$(OD) $(ODFLAGS) $(BUILD_DIR)/$(PROGRAM_NAME).elf > $(BUILD_DIR)/$(PROGRAM_NAME).list
	@echo ""
	@$(SIZE) -Ax $(BUILD_DIR)/$(PROGRAM_NAME).elf

$(PROGRAM_NAME).elf:$(C_OBJS) $(A_OBJS)
	@echo "Linking  to  " $@
	$(CC) $(LFLAGS) $(TARGET_ARCH) -o $(BUILD_DIR)/$(PROGRAM_NAME).elf $(C_OBJS) $(A_OBJS)

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling " $< "  to  " $@
	$(CC) $(CFLAGS) $(TARGET_ARCH) -c -o $@ $<

$(BUILD_DIR)/%.o: %.s
	@echo "Assembling " $< "  to  " $@
	$(AS) $(AFLAGS) $(TARGET_ARCH) -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.bin
	rm -f $(BUILD_DIR)/*.list
	rm -f $(BUILD_DIR)/*.map
	rm -f $(BUILD_DIR)/*.elf
