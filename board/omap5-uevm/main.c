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
    portTickType LastWake;

	trace_append("%s: starting task\n", __func__);

    /* Workaround
     *
     * We can't enable interrupts in U-Boot. Let's wait until Linux is booted
     * on CPU. Linux remoteproc framework sends a ping to IPU via mailbox when
     * its initialization is completed. This ping is a good indicator for IPU
     * that Linux and its remoteproc/rpmsg subsystems are up an running.
     */

    while (1) {
        msg = mailbox_read();

        if (msg == HOST_ECHO_REQUEST) {
            trace_append("%s: received echo request [0x%x] from CPU\n", __func__, msg);
            break;
        }

        trace_append("%s: host-to-m3 mailbox [0x%x]\n", __func__, msg);
        LastWake = xTaskGetTickCount();
        vTaskDelayUntil(&LastWake, 5000);
    }

	virtqueue_init();
	nvic_enable_irq(MAILBOX_IRQ);
	enable_mailbox_irq();

	trace_append("%s: start main loop\n", __func__);

	for (;;) {
		xQueueReceive(MboxQueue, &msg, portMAX_DELAY);

		switch(msg) {

		case RP_MBOX_ECHO_REQUEST :
			mailbox_send(M3_TO_HOST_MBX, RP_MBOX_ECHO_REPLY);
			break;

		case HOST_TO_M3_VRING :
			ret = virtqueue_get_avail_buf(&virtqueue_list[msg], &virtq_buf);

			/* make a local copy of the buffer */
			local_vq_buf = pvPortMalloc(RP_MSG_BUF_SIZE);
			memcpy(local_vq_buf, virtq_buf.buf_ptr, RP_MSG_BUF_SIZE);
			virtqueue_add_used_buf(&virtqueue_list[msg], virtq_buf.head);

			/* dispatch to the service queue */
			rpmsg_dispatch_msg(local_vq_buf);

			break;

		case M3_TO_HOST_VRING :
			xSemaphoreGive(InitDoneSemaphore);
			break;

        default:
	        trace_append("%s: unknown message\n", __func__);
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

	trace_append("%s: setup FreeRTOS...\n", __func__);

	MboxQueue = xQueueCreate( 32, sizeof( unsigned int* ) );
	vSemaphoreCreateBinary(InitDoneSemaphore);
	xSemaphoreTake(InitDoneSemaphore, portMAX_DELAY);

	hw_init();

	xTaskCreate(LedFlash, "led", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	xTaskCreate(IpcTask, "ipc", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(RdaemonTask, "rdaemon", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

	trace_append("%s: start FreeRTOS...\n", __func__);

	vTaskStartScheduler();

	/* Just sit and flash the LED quickly if we fail */

	while( 1 ) {
		for( i = 0; i < 50000; i++ );
        toggle_bit(GPIO_DATAOUT, 25);
	}
}
