#ifndef __REMOTEPROC_H__
#define __REMOTEPROC_H__

#include <stddef.h>
#include <stdint.h>

#include "common.h"

/*
 * These definitions come from <linux/remoteproc.h>.
 * They may change as the remote processor interface is updated.
 * Make sure they match the ones used by your current kernel source.
 */

struct resource_table {
	u32 ver;
	u32 num;
	u32 reserved[2];
	u32 offset[0];
} __packed;

struct fw_rsc_hdr {
	u32 type;
	u8 data[0];
} __packed;

struct fw_rsc_carveout {
	u32 da;
	u32 pa;
	u32 len;
	u32 flags;
	u32 memregion;
	u8 name[32];
} __packed;

struct fw_rsc_devmem {
	u32 da;
	u32 pa;
	u32 len;
	u32 flags;
	u32 memregion;
	u8 name[32];
} __packed;

struct fw_rsc_trace {
	u32 da;
	u32 len;
	u32 reserved;
	u8 name[32];
} __packed;

struct fw_rsc_vdev_vring {
	u32 da;
	u32 align;
	u32 num;
	u32 notifyid;
	u32 reserved;
} __packed;

struct fw_rsc_vdev {
	u32 id;
	u32 notifyid;
	u32 dfeatures;
	u32 gfeatures;
	u32 config_len;
	u8 status;
	u8 num_of_vrings;
	u8 reserved[2];
	struct fw_rsc_vdev_vring vring[0];
} __packed;

enum fw_resource_type {
	RSC_CARVEOUT	= 0,
	RSC_DEVMEM		= 1,
	RSC_TRACE		= 2,
	RSC_VDEV 		= 3,
	RSC_SUSPD_TIME	= 4,
	RSC_CUSTOM		= 5,
	RSC_LAST		= 6,
};

/*
 * Use flags to define memory access for each resource
 * Must match include/linux/rproc_drm.h in the kernel source tree
 */

enum rproc_memregion {
    RPROC_MEMREGION_UNKNOWN,
    RPROC_MEMREGION_2D,
    RPROC_MEMREGION_SMEM,
    RPROC_MEMREGION_CODE,
    RPROC_MEMREGION_DATA,
    RPROC_MEMREGION_1D,
    RPROC_MEMREGION_VRING
};


#endif /* __REMOTEPROC_H__ */
