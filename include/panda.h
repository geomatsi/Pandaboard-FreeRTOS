#ifndef __PANDA_H__
#define __PANDA_H__

#include <stddef.h>
#include <stdint.h>

#define GPIO1_DATAOUT           0xfff1013c
#define GPIO1_OE                0xfff10134

#define PID_REG                 0xE00FFFE0

#define L4_44XX_BASE            0x4a000000

#define L4_PERIPHERAL_L4CFG     (L4_44XX_BASE)
#define IPU_PERIPHERAL_L4CFG    0xAA000000

#define L4_PERIPHERAL_L4PER     0x48000000
#define IPU_PERIPHERAL_L4PER    0xA8000000

#define L4_PERIPHERAL_L4EMU     0x54000000
#define IPU_PERIPHERAL_L4EMU    0xB4000000

#define L3_PERIPHERAL_DMM       0x4E000000
#define IPU_PERIPHERAL_DMM      0xAE000000

#define L3_IVAHD_CONFIG         0x5A000000
#define IPU_IVAHD_CONFIG        0xBA000000

#define L3_IVAHD_SL2            0x5B000000
#define IPU_IVAHD_SL2           0xBB000000

#define L3_TILER_MODE_0_1       0x60000000
#define IPU_TILER_MODE_0_1      0x60000000

#define L3_TILER_MODE_2         0x70000000
#define IPU_TILER_MODE_2        0x70000000

#define L3_TILER_MODE_3         0x78000000
#define IPU_TILER_MODE_3        0x78000000

#define IPU_MEM_TEXT            0x0
#define IPU_MEM_DATA            0x80000000

#define IPU_MEM_IOBUFS          0x88000000

#define IPU_MEM_IPC_DATA        0x9F000000
#define IPU_MEM_IPC_VRING       0xA0000000
#define IPU_MEM_RPMSG_VRING0    0xA0000000
#define IPU_MEM_RPMSG_VRING1    0xA0004000
#define IPU_MEM_VRING_BUFS0     0xA0040000
#define IPU_MEM_VRING_BUFS1     0xA0080000

#define IPU_MEM_IPC_VRING_SIZE  SZ_1M
#define IPU_MEM_IPC_DATA_SIZE   SZ_1M
#define IPU_MEM_TEXT_SIZE       (SZ_1M * 6)
#define IPU_MEM_DATA_SIZE       (SZ_1M * 100)
#define IPU_MEM_IOBUFS_SIZE     (SZ_1M * 90)

/*
 * Assign fixed RAM addresses to facilitate a fixed MMU table.
 * PHYS_MEM_IPC_VRING & PHYS_MEM_IPC_DATA MUST be together.
 */
#define PHYS_MEM_IPC_VRING      0x99000000

#define PHYS_MEM_IOBUFS         0xBA300000

/*
 * Sizes of the virtqueues (expressed in number of buffers supported,
 * and must be power of 2)
 */
#define IPU_RPMSG_VQ0_SIZE      256
#define IPU_RPMSG_VQ1_SIZE      256

/* flip up bits whose indices represent features we support */
#define RPMSG_IPU_C0_FEATURES   1

#endif /* __PANDA_H__ */
