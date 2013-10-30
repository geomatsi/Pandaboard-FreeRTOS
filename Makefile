FREERTOS_SOURCE_DIR = FreeRTOS
APPLICATION_SOURCE_DIR = Application
BUILD_DIR = build

CROSS_COMPILE ?= arm-none-eabi-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

CFLAGS = -mthumb -mcpu=cortex-m3 -D GCC_ARMCM  -O2 -fno-stack-protector -fno-unwind-tables -fno-exceptions

CFLAGS += \
		 -I$(APPLICATION_SOURCE_DIR) \
		 -I$(FREERTOS_SOURCE_DIR)/Source/include \
		 -I$(FREERTOS_SOURCE_DIR)/Source/portable/GCC/ARM_CM3

LDFLAGS = -Tcortex-m3.lds
LIBGCC=$(shell $(CC) -mthumb -mcpu=cortex-m3 -print-libgcc-file-name)

FREERTOS_SOURCE_DIRS = \
	$(FREERTOS_SOURCE_DIR)/Source \
	$(FREERTOS_SOURCE_DIR)/Source/portable/GCC/ARM_CM3 \
	$(FREERTOS_SOURCE_DIR)/Source/portable/MemMang

VPATH = $(FREERTOS_SOURCE_DIRS):$(APPLICATION_SOURCE_DIR)
APPLICATION_OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/startup.o $(BUILD_DIR)/stdlib.o

OBJS =	$(BUILD_DIR)/list.o \
	$(BUILD_DIR)/queue.o \
	$(BUILD_DIR)/tasks.o \
	$(BUILD_DIR)/timers.o \
	$(BUILD_DIR)/croutine.o \
	$(BUILD_DIR)/port.o \
	$(BUILD_DIR)/heap_1.o \
	$(APPLICATION_OBJS)

all: $(BUILD_DIR) freertos-demo.out

-include $(OBJS:.o=.d)

freertos-demo.out: $(OBJS) cortex-m3.lds
	$(LD) $(LDFLAGS) $(OBJS) -o freertos-demo.out

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf $(BUILD_DIR)
	rm -f freertos-demo.out

.PHONY: clean
