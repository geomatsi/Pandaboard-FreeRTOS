#include <string.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

#include "common.h"
#include "mailbox.h"
#include "trace.h"
#include "interrupt.h"
#include "virtio.h"
#include "rdaemon.h"
#include "rpmsg.h"

#include "omap5-uevm.h"

/* */

xQueueHandle MboxQueue;
xSemaphoreHandle InitDoneSemaphore;

/* */

static void LedFlash(void *Parameters)
{
	portTickType LastWake;

	LastWake = xTaskGetTickCount();

	while(1) {
        toggle_bit(GPIO_DATAOUT, 25);
		vTaskDelayUntil(&LastWake, 500);
	}
}

/* */

static void IpcTask (void * pvParameters)
{
	unsigned int msg, *local_vq_buf;
	int ret;
	struct virtqueue_buf virtq_buf;

	virtqueue_init();
	nvic_enable_irq(MAILBOX_IRQ);
	enable_mailbox_irq();

	for (;;) {
		xQueueReceive(MboxQueue, &msg, portMAX_DELAY);

		switch(msg) {

		case RP_MBOX_ECHO_REQUEST :
	        trace_printf("IpcTask: RP_MBOX_ECHO_REQUEST\n");
			mailbox_send(M3_TO_HOST_MBX, RP_MBOX_ECHO_REPLY);
			break;

		case HOST_TO_M3_VRING :
	        trace_printf("IpcTask: HOST_TO_M3_VRING\n");
			ret = virtqueue_get_avail_buf(&virtqueue_list[msg], &virtq_buf);

			/* make a local copy of the buffer */
			local_vq_buf = pvPortMalloc(RP_MSG_BUF_SIZE);
			memcpy(local_vq_buf, virtq_buf.buf_ptr, RP_MSG_BUF_SIZE);
			virtqueue_add_used_buf(&virtqueue_list[msg], virtq_buf.head);

			/* dispatch to the service queue */
			rpmsg_dispatch_msg(local_vq_buf);

			break;

		case M3_TO_HOST_VRING :
	        trace_printf("IpcTask: M3_TO_HOST_VRING\n");
			xSemaphoreGive(InitDoneSemaphore);
			break;

        default:
	        trace_printf("IpcTask: unknown message\n");
            break;
		}
	}

	vTaskDelete(NULL);
}

/* */

static void hw_init(void)
{
    reset_bit(GPIO_OE, 25);
    reset_bit(GPIO_DATAOUT, 25);
}

/* */

int main()
{
	volatile int i;

	trace_printf("----> Setup FreeRTOS...\n");

	MboxQueue = xQueueCreate( 32, sizeof( unsigned int* ) );
	vSemaphoreCreateBinary(InitDoneSemaphore);
	xSemaphoreTake(InitDoneSemaphore, portMAX_DELAY);

	hw_init();

	xTaskCreate(LedFlash, "led", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	xTaskCreate(IpcTask, "ipc", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(RdaemonTask, "rdaemon", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

	trace_printf("----> Start FreeRTOS...\n");

	vTaskStartScheduler();

	/* Just sit and flash the LED quickly if we fail */

	while( 1 ) {
		for( i = 0; i < 200000; i++ );
        toggle_bit(GPIO_DATAOUT, 25);
	}
}
