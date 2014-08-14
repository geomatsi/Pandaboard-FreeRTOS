#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdint.h>
#include <string.h>

#include "rpmsg.h"
#include "rdaemon.h"

extern xSemaphoreHandle InitDoneSemaphore;
xQueueHandle RdaemonQueue;

void RdaemonTask(void *pvParameters)
{
    uint32_t client;
    struct service serv;
    struct rdaemon_msg_frame *payload;
    struct rpmsg_hdr *hdr;
    unsigned int *vq_buf;
    unsigned int len;

    RdaemonQueue = xQueueCreate( 32, sizeof( unsigned int* ) );

    strncpy(serv.name, "rpmsg-rdaemon", 32);
    serv.port = RDAEMON_PORT;
    serv.queue = &RdaemonQueue;

    xSemaphoreTake(InitDoneSemaphore, portMAX_DELAY);

    trace_append("%s: starting task... \n", __func__);

    rpmsg_service_register(&serv);

    xSemaphoreGive(InitDoneSemaphore);

    trace_append("%s: start main loop... \n", __func__);
    for (;;) {

        xQueueReceive(RdaemonQueue, &vq_buf, portMAX_DELAY);

        hdr = (struct rpmsg_hdr *) vq_buf;
        payload = (struct rdaemon_msg_frame *) &hdr->data;
        len = hdr->len;

        if (payload->msg_type == RDAEMON_PING) {

            client = payload->data;
            trace_append("%s: ping from client 0x%x\n", __func__, client);
            rpmsg_send_message(client);

        } else {

            trace_append("%s: message from client 0x%x [", __func__, client);
            trace_printf_len(payload, len);
            trace_printf("]\n");

            rpmsg_send_message(client);

        }

        vPortFree(vq_buf);
    }

    while(1);
}
