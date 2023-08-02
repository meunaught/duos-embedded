# Updated Makefile for DUOS: Last Updated on July 26, 2023

# Debug mode
DEBUG = 0

BUILD_DIR ?= build
TARGET_ELF ?= $(BUILD_DIR)/duos.elf
TARGET_BIN ?= $(BUILD_DIR)/duos.bin
TARGET_MAP ?= $(BUILD_DIR)/duos.map
SRC_DIRS = src/kern src/userland

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
LINKER := $(shell find $(SRC_DIRS) -name linker.ld)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC = arm-none-eabi-gcc
FPU = -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CPU = cortex-m4
CFLAGS= -mcpu=$(CPU) -mthumb $(FPU) $(INC_FLAGS) -MMD -MP -std=gnu17 -Wall -O0
LDFLAGS = -nostdlib -nostartfiles -nodefaultlibs -fno-exceptions -T $(LINKER) -Wl,-Map=$(TARGET_MAP) -O0

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2
endif

.PHONY: all clean probe flash

all : $(TARGET_ELF) $(TARGET_BIN)

$(TARGET_ELF): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_BIN) :
	arm-none-eabi-objcopy -O binary $(TARGET_ELF) $(TARGET_BIN)

clean:
	$(RM) -rv $(BUILD_DIR)

probe: 
	st-info --probe --connect-under-reset
flash:
	st-flash --reset write $(TARGET_BIN) 0x8000000

-include $(DEPS)

MKDIR_P ?= mkdir -p

