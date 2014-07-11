#ifndef __PANDA_H__
#define __PANDA_H__

#include <stddef.h>
#include <stdint.h>

/* */

#define GPIO_DATAOUT            0xfff0B13c
#define GPIO_OE                 0xfff0B134
#define PID_REG                 0xE00FFFE0
#define L4_44XX_BASE            0x4a000000

/* carveouts for text, data, traces */

#define IPU_MEM_TEXT            0x0
#define IPU_MEM_TEXT_SIZE       (SZ_1M * 6)

#define IPU_MEM_DATA            0x80000000
#define IPU_MEM_DATA_SIZE       (SZ_1M * 10)

#define IPU_MEM_IPC_DATA        0x9F000000
#define IPU_MEM_IPC_DATA_SIZE   SZ_1M

/* devmems for CPU peripherals: mailbox, gpio  */

#define L4_PERIPHERAL_L4CFG     (L4_44XX_BASE)
#define IPU_PERIPHERAL_L4CFG    0xAA000000

/* virtio */

#define IPU_MEM_IPC_VRING       0xA0000000
#define IPU_MEM_RPMSG_VRING0    0xA0000000
#define IPU_MEM_RPMSG_VRING1    0xA0004000

#define IPU_MEM_IPC_VRING_SIZE  SZ_1M

/*
 * Assign fixed RAM addresses to facilitate a fixed MMU table.
 * PHYS_MEM_IPC_VRING & PHYS_MEM_IPC_DATA MUST be together.
 */
#define PHYS_MEM_IPC_VRING      0x99000000

/*
 * Sizes of the virtqueues (expressed in number of buffers supported,
 * and must be power of 2)
 */
#define IPU_RPMSG_VQ0_SIZE      256
#define IPU_RPMSG_VQ1_SIZE      256

/* flip up bits whose indices represent features we support */
#define RPMSG_IPU_C0_FEATURES   1

#endif /* __PANDA_H__ */
