#include <stdint.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

volatile int *GPIO1_DATAOUT;
volatile int *GPIO1_OE;

static void LedFlash1(void *Parameters)
{
	portTickType LastWake;

	*GPIO1_OE &= ~(1 << 7);

	LastWake = xTaskGetTickCount();

	while(1) {
		*GPIO1_DATAOUT ^= (1 << 7);
		vTaskDelayUntil(&LastWake, 1000);
	}
}

static void LedFlash2(void *Parameters)
{
	portTickType LastWake;

	*GPIO1_OE &= ~(1 << 8);

	LastWake = xTaskGetTickCount();

	while(1) {
		*GPIO1_DATAOUT ^= (1 << 8);
		vTaskDelayUntil(&LastWake, 2000);
	}
}

void LedTest(int led, int duration)
{
	volatile int i, j;

	*GPIO1_OE &= ~(1 << led);

	for(j = 0; j < 10; j++) {
		for( i = 0; i < duration; i++ );
		*GPIO1_DATAOUT ^= (1 << led);
	}
}

static void hw_init(void)
{
	GPIO1_DATAOUT = (int *) 0xfff1013c;
	GPIO1_OE = (int *) 0xfff10134;

	*GPIO1_OE &= ~(1 << 7);
	*GPIO1_OE &= ~(1 << 8);

	*GPIO1_DATAOUT &= ~(1 << 7);
	*GPIO1_DATAOUT &= ~(1 << 8);

#if 0
	do {
		volatile uint32_t *ISR_ENA1 = (uint32_t *) 0xE000E100;

		*ISR_ENA1 = 0xf;
	} while (0);
#endif

	/* Blink LED D1: go and disable second m3 core :) */
	LedTest(7, 500000);
	*GPIO1_DATAOUT &= ~(1 << 7);
}

int main()
{
	volatile int i;

	hw_init();

	xTaskCreate(LedFlash1, (signed char *) "FlashD1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(LedFlash2, (signed char *) "FlashD2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
	vTaskStartScheduler();

	/* Just sit and flash the LED quickly if we fail */

	*GPIO1_DATAOUT &= ~(1 << 7);
	*GPIO1_DATAOUT |= (1 << 8);

	while( 1 ) {
		for( i = 0; i < 200000; i++ );
		*GPIO1_DATAOUT ^= (1 << 7);
		*GPIO1_DATAOUT ^= (1 << 8);
	}
}
