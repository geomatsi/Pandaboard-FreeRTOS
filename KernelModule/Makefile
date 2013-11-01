default: all

ARCH ?= arm
KERNEL ?= /usr/src/linux
CROSS_COMPILE ?= arm-none-linux-gnueabi-

MAKE = make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}

PWD = `pwd`

obj-m := remoteproc_ctl.o rpmsg_test.o

all:
	${MAKE} -C ${KERNEL}  M=${PWD} modules

clean:
	${MAKE} -C ${KERNEL}  M=${PWD} clean
	rm -rf Module.symvers modules.order
