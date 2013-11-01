#ifndef __RPMSG_H__
#define __RPMSG_H__

/* Virtio Ids: keep in sync with the linux "include/linux/virtio_ids.h" */
#define VIRTIO_ID_CONSOLE       3 /* virtio console */
#define VIRTIO_ID_RPMSG         7 /* virtio remote processor messaging */

/* Indices of rpmsg virtio features we support */
#define VIRTIO_RPMSG_F_NS       0  /* RP supports name service notifications */
#define VIRTIO_RING_F_SYMMETRIC 30 /* We support symmetric vring */

/* */

#include "FreeRTOS.h"
#include "queue.h"

#include "common.h"

struct service {
	char name[32];
	unsigned int port;
	xQueueHandle *queue;
};

/* taken from linux/rpmsg.h */
struct rpmsg_hdr {
    u32 src;
    u32 dst;
    u32 reserved;
    u16 len;
    u16 flags;
    u8 data[0];
} __packed;


struct rpmsg_ns_msg {
    char name[32];      /* name of service including 0 */
    unsigned int addr;  /* address of the service */
    unsigned int flags; /* see below */
} __packed;

enum rpmsg_ns_flags {
    RPMSG_NS_CREATE = 0,
    RPMSG_NS_DESTROY = 1
};

void rpmsg_service_register(struct service *serv);

#endif /* __RPMSG_H__ */
