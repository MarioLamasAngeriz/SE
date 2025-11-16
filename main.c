#include "includes/MKL46Z4.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "includes/lcd.h"
#include <stdint.h>

uint16_t datosArbitrarios[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

uint16_t productores = 0;
uint16_t consumidores = 0;
uint32_t mensajes_cola = 0;

void delay (uint32_t ms) {
	volatile int i;
	for (i = 0; i < ms * 1000; i++);
}

void buttons_init(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	PORTC->PCR[3] |= PORT_PCR_MUX(1);
	PORTC->PCR[3] |= PORT_PCR_PE_MASK;
	PORTC->PCR[3] |= PORT_PCR_PS_MASK;
	PORTC->PCR[3] |= PORT_PCR_IRQC(0x0A);
	GPIOC->PDDR &= ~(1 << 3);
				 
	PORTC->PCR[12] |= PORT_PCR_MUX(1);
	PORTC->PCR[12] |= PORT_PCR_PE_MASK;
	PORTC->PCR[12] |= PORT_PCR_PS_MASK; 
	PORTC->PCR[12] |= PORT_PCR_IRQC(0x0A);
	GPIOC->PDDR &= ~(1 << 12);
}

void queue_init(void) {

}

void conmutar_consumidores(void) {
	if (consumidores < 5) {
		consumidores++;
	} else {
		consumidores = 0;
	}
}

void conmutar_productores(void) {
	if (productores < 5) {
		productores++;
	} else {
		productores = 0;
	}
}

void display_info(void) {

	lcd_set(mensajes_cola / 10, 1);
	lcd_set(mensajes_cola % 10, 2);

	lcd_set(productores, 3);
	lcd_set(consumidores, 4);

}

void main_loop(void) {

	while (1) {
		display_info();
		delay(1000);
	}

}

int main(void) {

	lcd_ini();
	buttons_init();

	main_loop();

	return 0;

}

/*
void taskLedGreen(void *pvParameters)
{
    for (;;) {
        led_green_toggle();
        vTaskDelay(200/portTICK_RATE_MS);
    }
}

void taskLedRed(void *pvParameters)
{
    for (;;) {
        led_red_toggle();
        vTaskDelay(500/portTICK_RATE_MS);
    }
}

	* create green led task *
	xTaskCreate(taskLedGreen, (signed char *)"TaskLedGreen", 
		configMINIMAL_STACK_SIZE, (void *)NULL, 1, NULL);

	* create red led task *
	xTaskCreate(taskLedRed, (signed char *)"TaskLedRed", 
		configMINIMAL_STACK_SIZE, (void *)NULL, 1, NULL);
	
	* start the scheduler *
	vTaskStartScheduler();

	* should never reach here! *
	for (;;);
*/
