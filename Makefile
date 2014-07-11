FREERTOS_SOURCE_DIR = /home/matsi/devel/ti/omap5-evm/FreeRTOSV7.3.0/FreeRTOS

APPLICATION_INCLUDE_DIR = include
BUILD_DIR = build

#CROSS_COMPILE = /home/matsi/devel/tools/CodeSourcery/2014.05-17-arm/bin/arm-none-eabi-
CROSS_COMPILE = /home/matsi/devel/tools/CodeSourcery/Sourcery_CodeBench_for_ARM_GNU_Linux_2012.03-47/bin/arm-none-linux-gnueabi-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

CFLAGS = -mthumb -mcpu=cortex-m3 -D GCC_ARMCM  -O2 -fno-stack-protector -fno-unwind-tables -fno-exceptions

CFLAGS += \
		 -I$(APPLICATION_INCLUDE_DIR) \
		 -I$(FREERTOS_SOURCE_DIR)/Source/include \
		 -I$(FREERTOS_SOURCE_DIR)/Source/portable/GCC/ARM_CM3

LDFLAGS = -Tcortex-m3.lds
LIBGCC=$(shell $(CC) -mthumb -mcpu=cortex-m3 -print-libgcc-file-name)

APPLICATION_SOURCE_DIRS = \
	src

FREERTOS_SOURCE_DIRS = \
	$(FREERTOS_SOURCE_DIR)/Source \
	$(FREERTOS_SOURCE_DIR)/Source/portable/GCC/ARM_CM3 \
	$(FREERTOS_SOURCE_DIR)/Source/portable/MemMang

VPATH = $(FREERTOS_SOURCE_DIRS):$(APPLICATION_SOURCE_DIRS)

COMMON_OBJS = \
	$(BUILD_DIR)/trace.o \
	$(BUILD_DIR)/rpmsg.o \
	$(BUILD_DIR)/virtio.o \
	$(BUILD_DIR)/mailbox.o \
	$(BUILD_DIR)/rdaemon.o \
	$(BUILD_DIR)/interrupt.o \
	$(BUILD_DIR)/stdlib.o

FREERTOS_OBJS =	\
	$(BUILD_DIR)/list.o \
	$(BUILD_DIR)/queue.o \
	$(BUILD_DIR)/tasks.o \
	$(BUILD_DIR)/timers.o \
	$(BUILD_DIR)/croutine.o \
	$(BUILD_DIR)/port.o \
	$(BUILD_DIR)/heap_2.o

OBJS = \
	$(COMMON_OBJS) \
	$(FREERTOS_OBJS)

OMAP5_UEVM_OBJS = \
	$(BUILD_DIR)/omap5-uevm/startup.o \
	$(BUILD_DIR)/omap5-uevm/main.o \
	$(BUILD_DIR)/omap5-uevm/rsc_table.o

OMAP4_PANDA_OBJS = \
	$(BUILD_DIR)/omap4-panda/startup.o \
	$(BUILD_DIR)/omap4-panda/main.o \
	$(BUILD_DIR)/omap4-panda/rsc_table.o

all: $(BUILD_DIR) omap5-uevm omap4-panda

omap5-uevm: $(OMAP5_UEVM_OBJS) $(OBJS) cortex-m3.lds
	$(LD) $(LDFLAGS) $(OBJS) $(OMAP5_UEVM_OBJS) -o freertos-demo.omap5-uevm.out

omap4-panda: $(OMAP4_PANDA_OBJS) $(OBJS) cortex-m3.lds
	$(LD) $(LDFLAGS) $(OBJS) $(OMAP4_PANDA_OBJS) -o freertos-demo.omap4-panda.out

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/omap5-uevm
	mkdir $(BUILD_DIR)/omap4-panda

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/omap5-uevm/%.o: board/omap5-uevm/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/omap4-panda/%.o: board/omap4-panda/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f freertos-demo.*.out

.PHONY: clean
