#include "common.h"
#include "remoteproc.h"
#include "trace.h"
#include "mailbox.h"
#include "interrupt.h"
#include "virtio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "omap5-uevm.h"

/* */

#define STACK_SIZE 256

static unsigned long Stack[STACK_SIZE];

/* */

void ResetISR(void);

/* */

extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);
extern void vPortSVCHandler( void );

/* */

extern xQueueHandle MboxQueue;
extern int main(void);

/* */

void ResetISR(void)
{
    /* if we are core1, go to sleep */
    if (read32(PID_REG) != 0)
        asm volatile ("wfi");

    main();
}

static void NmiISR(void)
{
	volatile int i;

	trace_printf("NMI handler\n");

    reset_bit(GPIO_OE, 25);
    reset_bit(GPIO_DATAOUT, 25);

	while (1) {
		for( i = 0; i < 10000; i++);
        toggle_bit(GPIO_DATAOUT, 25);
	}
}

static void FaultISR(void)
{
	volatile int i;

	trace_printf("FAULT handler\n");

    reset_bit(GPIO_OE, 25);
    reset_bit(GPIO_DATAOUT, 25);

	while (1) {
		for( i = 0; i < 10000; i++);
        toggle_bit(GPIO_DATAOUT, 25);
	}
}

static void DefaultISR(void)
{
	volatile int i;

	trace_printf("DEFAULT handler\n");

    reset_bit(GPIO_OE, 25);
    reset_bit(GPIO_DATAOUT, 25);

	while (1) {
		for( i = 0; i < 10000; i++);
        toggle_bit(GPIO_DATAOUT, 25);
	}
}

static void MailBoxHandler(void)
{
    unsigned int msg, nb_msg;

	trace_printf("MailBox handler\n");

    nb_msg = mailbox_get_status();

    while (nb_msg > 0) {
        msg = mailbox_read();
        xQueueSend(MboxQueue, &msg, portMAX_DELAY);
        nb_msg--;
    }

    /* clear the NVIC mailbox irq */
    nvic_clear_irq(MAILBOX_IRQ);
}

/* */

__attribute__ ((section(".isr_vector")))
void (* const Vectors[])(void) = {
	(void (*)(void))((unsigned long) Stack + sizeof(Stack)),
	ResetISR,			// Reset
	NmiISR,				// NMI
	FaultISR,  			// Hard fault
	DefaultISR,			// MPU fault
	DefaultISR,			// Bus fault
	DefaultISR,			// Usage fault
	0, 0, 0, 0,			// reserved slots
	vPortSVCHandler,	// SVCall handler
	DefaultISR,			// Debug
	0,					// reserved slot
	xPortPendSVHandler,	// Pendable SV
	xPortSysTickHandler,// SysTick

	/* External (vendor specific) handlers */

	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	MailBoxHandler,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
	DefaultISR,
};

