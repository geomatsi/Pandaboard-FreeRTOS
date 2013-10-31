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

struct service {
	char name[32];
	unsigned int port;
	xQueueHandle *queue;
};

void rpmsg_service_register(struct service *serv);

#endif /* __RPMSG_H__ */
