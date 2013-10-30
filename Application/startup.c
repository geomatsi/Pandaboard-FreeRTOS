#include "remoteproc.h"
#include "common.h"
#include "panda.h"

/* */

#define STACK_SIZE 256

static unsigned long Stack[STACK_SIZE];

/* */

void ResetISR(void);

static void NmiISR(void);
static void TestISR(void);
static void FaultISR(void);
static void DefaultISR(void);

extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);
extern void vPortSVCHandler( void );

extern int main(void);

/* Description of the resources required by this firmware image.
 * Currently we simply map 1M of pyhsical memory from PA 0xb0000000
 * to the device address 0x0 to hold our text/data sections.
 * We also map a 1M block containing the GPIO1 perihperal registers (PA 0x4a300000)
 * to a device address 0xfff00000 so we can flash a LED.
 * */

struct carveout_entry {
    unsigned int type;
    struct fw_rsc_carveout data;
};

struct devmem_entry {
    unsigned int type;
    struct fw_rsc_devmem data;
};

struct resource_table_ducati {
    unsigned int version;
    unsigned int num;
    unsigned int reserved[2];
    unsigned int offset[2];

    /* */

    struct carveout_entry carveout1;
    struct devmem_entry devmem1;

};

__attribute__ ((section (".resource_table")))
volatile struct resource_table_ducati resources = {
    1,
    2,
    { 0 },

    {
        offsetof(struct resource_table_ducati, carveout1),
        offsetof(struct resource_table_ducati, devmem1)
    },

    {
        RSC_CARVEOUT,
        { 0x0, 0xb0000000, 0x100000, 0x0, 0x0, "text" }
    },

    {
        RSC_DEVMEM,
        { 0xfff00000, 0x4a300000, 0x100000, 0x0, 0x0, "gpio1" }
    }
};

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
};

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

    reset_bit(GPIO1_OE, 7);
    reset_bit(GPIO1_OE, 8);

    reset_bit(GPIO1_DATAOUT, 7);
    reset_bit(GPIO1_DATAOUT, 8);

	while (1) {
		for( i = 0; i < 10000; i++);
        toggle_bit(GPIO1_DATAOUT, 7);
	}
}

static void FaultISR(void)
{
	volatile int i;

    reset_bit(GPIO1_OE, 7);
    reset_bit(GPIO1_OE, 8);

    reset_bit(GPIO1_DATAOUT, 7);
    reset_bit(GPIO1_DATAOUT, 8);

	while (1) {
		for( i = 0; i < 10000; i++);
        toggle_bit(GPIO1_DATAOUT, 8);
	}
}

static void DefaultISR(void)
{
	volatile int i;

    reset_bit(GPIO1_OE, 7);
    reset_bit(GPIO1_OE, 8);

    reset_bit(GPIO1_DATAOUT, 7);
    reset_bit(GPIO1_DATAOUT, 8);

	while (1) {
		for( i = 0; i < 10000; i++);
        toggle_bit(GPIO1_DATAOUT, 7);
        toggle_bit(GPIO1_DATAOUT, 8);
	}
}

static void TestISR(void)
{
	volatile int i;

    reset_bit(GPIO1_OE, 8);

	while(1) {
		for( i = 0; i < 10000; i++);
         toggle_bit(GPIO1_DATAOUT, 8);
	}
}
