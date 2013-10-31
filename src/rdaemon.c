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
        xQueueReceive(RdaemonQueue, &payload, portMAX_DELAY);

        if (payload->msg_type == RDAEMON_PING) {

            client = payload->data;
            rpmsg_send_message(client);

        } else {

            rpmsg_send_message(client);

        }
    }

    while(1);
}
