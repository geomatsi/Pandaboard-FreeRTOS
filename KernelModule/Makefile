default: rproc

ARCH ?= arm
KERNEL ?= /usr/src/linux
CROSS_COMPILE ?= arm-none-linux-gnueabi-

MAKE = make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}

PWD = `pwd`

obj-m := rproc.o

rproc:
	${MAKE} -C ${KERNEL}  M=${PWD} modules

clean:
	${MAKE} -C ${KERNEL}  M=${PWD} clean
	rm -rf Module.symvers modules.order
