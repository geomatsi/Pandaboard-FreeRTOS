#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "common.h"
#include "panda.h"

static void LedFlash1(void *Parameters)
{
	portTickType LastWake;

	LastWake = xTaskGetTickCount();

	while(1) {
        toggle_bit(GPIO1_DATAOUT, 7);
		vTaskDelayUntil(&LastWake, 1000);
	}
}

static void LedFlash2(void *Parameters)
{
	portTickType LastWake;

	LastWake = xTaskGetTickCount();

	while(1) {
        toggle_bit(GPIO1_DATAOUT, 8);
		vTaskDelayUntil(&LastWake, 2000);
	}
}

void LedTest(int led, int duration)
{
	volatile int i, j;

	for(j = 0; j < 10; j++) {
		for( i = 0; i < duration; i++ );
        toggle_bit(GPIO1_DATAOUT, led);
	}
}

static void hw_init(void)
{
    reset_bit(GPIO1_OE, 7);
    reset_bit(GPIO1_OE, 8);

    reset_bit(GPIO1_DATAOUT, 7);
    reset_bit(GPIO1_DATAOUT, 8);
}

int main()
{
	volatile int i;

	hw_init();

	xTaskCreate(LedFlash1, (signed char *) "FlashD1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(LedFlash2, (signed char *) "FlashD2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
	vTaskStartScheduler();

	/* Just sit and flash the LED quickly if we fail */

    reset_bit(GPIO1_DATAOUT, 7);
    set_bit(GPIO1_DATAOUT, 8);

	while( 1 ) {
		for( i = 0; i < 200000; i++ );
        toggle_bit(GPIO1_DATAOUT, 7);
        toggle_bit(GPIO1_DATAOUT, 8);
	}
}
