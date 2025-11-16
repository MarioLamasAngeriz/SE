#define COLA 99
#include <stdint.h>
#include "includes/MKL46Z4.h"
#include "freertos/FreeRTOS.h"
#include "includes/lcd.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/projdefs.h"

uint16_t datosArbitrarios[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

TaskHandle_t array_productores[5];
TaskHandle_t array_consumidores[5];

uint16_t productores = 0;
uint16_t consumidores = 0;

uint32_t mensajes_cola = 0;
QueueHandle_t colaMensajes;

typedef struct {
	uint16_t indice;
	uint16_t dato;
} mensaje_t;

void queue_init(void) {
	colaMensajes = xQueueCreate(COLA, sizeof(mensaje_t));	//comprobar correcto?
}

void iniciar_reloxos(void) {
	MCG->C1 = MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
	MCG->C2 &= ~MCG_C2_IRCS_MASK; //0 32KHZ internal reference clock; 1= 4MHz irc
}

void dato_enviado(void) {
	GPIOD->PCOR = (1 << 5);
	vTaskDelay(pdMS_TO_TICKS(250));;
	GPIOD->PSOR = (1 << 5);
}

void dato_recibido(void) {
	GPIOE->PCOR = (1 << 29);
	vTaskDelay(pdMS_TO_TICKS(250));
	GPIOE->PSOR = (1 << 29);
}

void init_arrays(void) {
	for (uint16_t i = 0; i < 5; i++) {
		array_consumidores[i] = NULL;
		array_productores[i] = NULL;
	}
}

void leds_init(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD->PCR[5] |= PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1 << 5);
	GPIOD->PSOR |= (1 << 5);

	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[29] |= PORT_PCR_MUX(1);
	GPIOE->PDDR |= (1 << 29);
	GPIOE->PSOR |= (1 << 29);
}

void buttons_init(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	PORTC->PCR[3] |= PORT_PCR_MUX(1);
	PORTC->PCR[3] |= PORT_PCR_PE_MASK;
	PORTC->PCR[3] |= PORT_PCR_PS_MASK;
	GPIOC->PDDR &= ~(1 << 3);
				
	PORTC->PCR[12] |= PORT_PCR_MUX(1);
	PORTC->PCR[12] |= PORT_PCR_PE_MASK;
	PORTC->PCR[12] |= PORT_PCR_PS_MASK; 
	GPIOC->PDDR &= ~(1 << 12);
}

void display_info(void) {
	mensajes_cola = uxQueueMessagesWaiting(colaMensajes);
	lcd_set(mensajes_cola / 10, 1);
	lcd_set(mensajes_cola % 10, 2);
	lcd_set(productores, 3);
	lcd_set(consumidores, 4);
}

void productor(void* pvParameters) {

	uint16_t indice = 0;
	uint16_t dato;
	mensaje_t mensajeEnviado;
	while (1) {
			
		if (indice > 9) 
			indice = 0;
	
		dato = datosArbitrarios[indice];
		mensajeEnviado.indice = indice;
		mensajeEnviado.dato = dato;
		indice++;
	
		//envia mensaje a la cola, espera 100 ticks si llena
		if (xQueueSend(colaMensajes, &mensajeEnviado, pdMS_TO_TICKS(100)) == pdPASS) {
			dato_enviado();
			display_info();
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void consumidor(void* pvParameters) {

	mensaje_t mensajeRecibido;
	while (1) {
		//espera infinito por mensaje
		if (xQueueReceive(colaMensajes, &mensajeRecibido, portMAX_DELAY) == pdPASS) {
		
			uint16_t indice = mensajeRecibido.indice;
			uint16_t dato = mensajeRecibido.dato;

			if (dato == datosArbitrarios[indice]) {
				dato_recibido();
				display_info();
			} 
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}

void conmutar_consumidores(void) {
	if (consumidores < 5) {
		xTaskCreate(consumidor, "Consumidor", configMINIMAL_STACK_SIZE, NULL, 1, &array_consumidores[consumidores]);
		consumidores++;
	} else {
		for (uint16_t i = 0; i < 5; i++) {
			if (array_consumidores[i] != NULL) {		
				vTaskDelete(array_consumidores[i]);
				array_consumidores[i] = NULL;
			}
		}
		consumidores = 0;
	}
}

void conmutar_productores(void) {
	if (productores < 5) {
		xTaskCreate(productor, "Productor", configMINIMAL_STACK_SIZE, NULL, 1, &array_productores[productores]);
		productores++;
	} else {
		for (uint16_t i = 0; i < 5; i++) {
			if (array_productores[i] != NULL) {
				vTaskDelete(array_productores[i]);
				array_productores[i] = NULL;
			}
		}
		productores = 0;
	}
}

void main_loop(void * pvParameters) {

	uint16_t but_izq_prev = 0;
	uint16_t but_der_prev = 0;

	while (1) {

		uint16_t but_izq = !(GPIOC->PDIR & (1 << 3));
		uint16_t but_der = !(GPIOC->PDIR & (1 << 12));

		if (but_izq && !but_izq_prev) {
			conmutar_consumidores();
			display_info();
		}

		if (but_der && !but_der_prev) {
			conmutar_productores();
			display_info();
		}

		display_info();

		but_izq_prev = but_izq;
		but_der_prev = but_der;

		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

int main(void) {

	lcd_ini();
	leds_init();
	queue_init();
	init_arrays();
	buttons_init();
	iniciar_reloxos();

	xTaskCreate(main_loop, "main_loop", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	//inicia FreeRTOS
	vTaskStartScheduler();

	for(;;);

	return 0;

}

