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

    RdaemonQueue = xQueueCreate( 32, sizeof( unsigned int* ) );

    strncpy(serv.name, "rpmsg-rdaemon", 32);
    serv.port = RDAEMON_PORT;
    serv.queue = &RdaemonQueue;

    xSemaphoreTake(InitDoneSemaphore, portMAX_DELAY);

    trace_printf("registering rdaemon task... \n");

    rpmsg_service_register(&serv);

    xSemaphoreGive(InitDoneSemaphore);

    trace_printf("rdaemon loop started... \n");
    for (;;) {

        xQueueReceive(RdaemonQueue, &vq_buf, portMAX_DELAY);

        hdr = (struct rpmsg_hdr *) vq_buf;
        payload = (struct rdaemon_msg_frame *) &hdr->data;

        if (payload->msg_type == RDAEMON_PING) {

            trace_printf("rdaemon: ping...\n");
            client = payload->data;
            rpmsg_send_message(client);

        } else {

            trace_printf("rdaemon : ");
            trace_printf(payload);
            trace_printf("\n");

            rpmsg_send_message(client);

        }

        vPortFree(vq_buf);
    }

    while(1);
}
