/*
 * Copyright (c) 2011-2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "common.h"
#include "panda.h"

#include "remoteproc.h"
#include "rpmsg.h"

/* */

struct carveout_entry {
    unsigned int type;
    struct fw_rsc_carveout data;
};

struct devmem_entry {
    unsigned int type;
    struct fw_rsc_devmem data;
};

struct trace_entry {
    unsigned int  type;
    struct fw_rsc_trace data;
};

struct vdev_entry {
    unsigned int  type;
    struct fw_rsc_vdev data;
};


/* */

struct resource_table_ducati {
    unsigned int version;
    unsigned int num;
    unsigned int reserved[2];
    unsigned int offset[16];

    /* rpmsg vdev entry */
    struct vdev_entry rpmsg_vdev;
    struct fw_rsc_vdev_vring rpmsg_vring0;
    struct fw_rsc_vdev_vring rpmsg_vring1;

    /* text carveout entry */
    struct carveout_entry text_cout;

    /* data carveout entry */
    struct carveout_entry data_cout;

    /* ipcdata carveout entry */
    struct carveout_entry ipcdata_cout;

    /* trace entry */
    struct trace_entry trace;

    /* devmem entry */
    struct devmem_entry devmem0;

    /* devmem entry */
    struct devmem_entry devmem1;

    /* devmem entry */
    struct devmem_entry devmem2;

    /* devmem entry */
    struct devmem_entry devmem3;

    /* devmem entry */
    struct devmem_entry devmem4;

    /* devmem entry */
    struct devmem_entry devmem5;

    /* devmem entry */
    struct devmem_entry devmem6;

    /* devmem entry */
    struct devmem_entry devmem7;

    /* devmem entry */
    struct devmem_entry devmem8;

    /* devmem entry */
    struct devmem_entry devmem9;

    /* devmem entry */
    struct devmem_entry devmem10;
};

__attribute__ ((section (".resource_table")))
volatile struct resource_table_ducati resources = {
    1,
    16,
    { 0 },

    {
        offsetof(struct resource_table_ducati, rpmsg_vdev),
        offsetof(struct resource_table_ducati, text_cout),
        offsetof(struct resource_table_ducati, data_cout),
        offsetof(struct resource_table_ducati, ipcdata_cout),
        offsetof(struct resource_table_ducati, trace),
        offsetof(struct resource_table_ducati, devmem0),
        offsetof(struct resource_table_ducati, devmem1),
        offsetof(struct resource_table_ducati, devmem2),
        offsetof(struct resource_table_ducati, devmem3),
        offsetof(struct resource_table_ducati, devmem4),
        offsetof(struct resource_table_ducati, devmem5),
        offsetof(struct resource_table_ducati, devmem6),
        offsetof(struct resource_table_ducati, devmem7),
        offsetof(struct resource_table_ducati, devmem8),
        offsetof(struct resource_table_ducati, devmem9),
        offsetof(struct resource_table_ducati, devmem10),
    },

    /* rpmsg vdev entry */
    {
        RSC_VDEV, { VIRTIO_ID_RPMSG, 0, RPMSG_IPU_C0_FEATURES, 0, 0, 0, 2, { 0, 0 } }
    },

    /* the two vrings */
    { IPU_MEM_RPMSG_VRING0, 4096, IPU_RPMSG_VQ0_SIZE, 1, 0 },
    { IPU_MEM_RPMSG_VRING1, 4096, IPU_RPMSG_VQ1_SIZE, 2, 0 },

    {
        RSC_CARVEOUT, { IPU_MEM_TEXT, 0, IPU_MEM_TEXT_SIZE, 0, 0, "IPU_MEM_TEXT" }
    },

    {
        RSC_CARVEOUT, {IPU_MEM_DATA, 0, IPU_MEM_DATA_SIZE, 0, 0, "IPU_MEM_DATA" }
    },

    {
        RSC_CARVEOUT, { IPU_MEM_IPC_DATA, 0, IPU_MEM_IPC_DATA_SIZE, 0, 0, "IPU_MEM_IPC_DATA" }
    },

    {
        RSC_TRACE, { 0x9f000000, 0x2800, 0, "trace:sysm3" }
    },

    {
        RSC_DEVMEM, { IPU_MEM_IPC_VRING, PHYS_MEM_IPC_VRING, IPU_MEM_IPC_VRING_SIZE, 0, 0, "IPU_MEM_IPC_VRING" }
    },

    {
        RSC_DEVMEM, { IPU_MEM_IOBUFS, PHYS_MEM_IOBUFS, IPU_MEM_IOBUFS_SIZE, 0, 0, "IPU_MEM_IOBUFS" }
    },

    {
        RSC_DEVMEM, { IPU_TILER_MODE_0_1, L3_TILER_MODE_0_1, SZ_256M, 0, 0, "IPU_TILER_MODE_0_1" }
    },

    {
        RSC_DEVMEM, { IPU_TILER_MODE_2, L3_TILER_MODE_2, SZ_128M, 0, 0, "IPU_TILER_MODE_2" }
    },

    {
        RSC_DEVMEM, { IPU_TILER_MODE_3, L3_TILER_MODE_3, SZ_128M, 0, 0, "IPU_TILER_MODE_3" }
    },

    {
        RSC_DEVMEM, { IPU_PERIPHERAL_L4CFG, L4_PERIPHERAL_L4CFG, SZ_16M, 0, 0, "IPU_PERIPHERAL_L4CFG" }
    },

    {
        RSC_DEVMEM, { IPU_PERIPHERAL_L4PER, L4_PERIPHERAL_L4PER, SZ_16M, 0, 0, "IPU_PERIPHERAL_L4PER" }
    },

    {
        RSC_DEVMEM, { IPU_IVAHD_CONFIG, L3_IVAHD_CONFIG, SZ_16M, 0, 0, "IPU_IVAHD_CONFIG" }
    },

    {
        RSC_DEVMEM, { IPU_IVAHD_SL2, L3_IVAHD_SL2, SZ_16M, 0, 0, "IPU_IVAHD_SL2" }
    },

    {
        RSC_DEVMEM, { IPU_PERIPHERAL_DMM, L3_PERIPHERAL_DMM, SZ_1M, 0, 0, "IPU_PERIPHERAL_DMM" }
    },

    {
        RSC_DEVMEM, { 0xfff00000, 0x4a300000, 0x100000, 0, 0, "GPIO" }
    },
};
