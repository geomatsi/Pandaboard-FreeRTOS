#ifndef __PANDA_H__
#define __PANDA_H__

#include <stddef.h>
#include <stdint.h>

/* */

#define PID_REG                 0xE00FFFE0

/* carveouts for text, data, traces */

#define IPU_MEM_TEXT            0x0
#define IPU_MEM_TEXT_SIZE       (SZ_1M * 6)

#define IPU_MEM_DATA            0x80000000
#define IPU_MEM_DATA_SIZE       (SZ_1M * 10)

#define IPU_MEM_IPC_DATA        0x9F000000
#define IPU_MEM_IPC_DATA_SIZE   SZ_1M

/* devmem: mailbox */

#define CPU_PERIPHERAL_L4CFG    0x4a000000
#define IPU_PERIPHERAL_L4CFG    0x700000
#define IPU_L4CFG_SIZE          0x100000

/* devmem: gpio */

#define CPU_PERIPHERAL_GPIO     0x48000000
#define IPU_PERIPHERAL_GPIO     0x600000
#define IPU_GPIO_SIZE           0x100000

#define GPIO_DATAOUT            0x65b13c
#define GPIO_OE                 0x65b134

/* virtio */

#define IPU_MEM_IPC_VRING       0xA0000000
#define IPU_MEM_RPMSG_VRING0    0xA0000000
#define IPU_MEM_RPMSG_VRING1    0xA0004000

#define IPU_MEM_IPC_VRING_SIZE  SZ_1M

/*
 * Assign fixed RAM addresses to facilitate a fixed MMU table.
 * PHYS_MEM_IPC_VRING & PHYS_MEM_IPC_DATA MUST be together.
 */
#define PHYS_MEM_IPC_VRING      0x95800000

/*
 * Sizes of the virtqueues (expressed in number of buffers supported,
 * and must be power of 2)
 */
#define IPU_RPMSG_VQ0_SIZE      256
#define IPU_RPMSG_VQ1_SIZE      256

/* flip up bits whose indices represent features we support */
#define RPMSG_IPU_C0_FEATURES   1

#endif /* __PANDA_H__ */
