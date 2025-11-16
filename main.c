#include <stdint.h>
#include "includes/MKL46Z4.h"
#include "freertos/FreeRTOS.h"
#include "includes/lcd.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define COLA 99


uint16_t datosArbitrarios[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

uint16_t productores = 0;
uint16_t consumidores = 0;
uint32_t mensajes_cola = 0;

QueueHandle_t colaMensajes;

typedef struct {
	uint16_t indice;
	uint16_t dato;
} mensaje_t;

void queue_init(void) {
	colaMensajes = xQueueCreate(COLA, sizeof(mensaje_t));
}

void productor(void) {

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
			//fallo al enviar
		} else {
			//enviado correctamente
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void consumidor(void) {

	mensaje_t mensajeRecibido;
	
	while (1) {
		//espera infinito por mensaje
		if (xQueueReceive(colaMensajes, &mensajeRecibido, portMAX_DELAY) == pdPASS) {
		
			uint16_t indice = mensajeRecibido.indice;
			uint16_t dato = mensajeRecibido.dato;

		
			vTaskDelay(pdMS_TO_TICKS(1000));

			if (dato == datosArbitrarios[indice]) {
				//mensaje exitoso!
			} else {
				//fallo al pasar el mensaje
			}
		}
	}
}

void conmutar_consumidores(void) {
	if (consumidores < 5) {
		consumidores++;
		//consumidor add
	} else {
		consumidores = 0;
		//consumidor delete all
	}
}

void conmutar_productores(void) {
	if (productores < 5) {
		productores++;
		//productor add
	} else {
		productores = 0;
		//productor delete all
	}
}

void display_info(void) {
	lcd_set(mensajes_cola / 10, 1);
	lcd_set(mensajes_cola % 10, 2);
	lcd_set(productores, 3);
	lcd_set(consumidores, 4);
}

void delay (volatile uint32_t ms) {
	volatile int i;
	for (i = 0; i < ms * 1000; i++);
}

void irclk_ini(void) {
	MCG->C1 = MCG_C1_IRCLKEN(1) | MCG_C1_IREFSTEN(1);
	MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
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

	NVIC_SetPriority(PORTC_PORTD_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

void PORTDIntHandler (void) {
	if (PORTC->ISFR & (1 << 3)) {
		conmutar_consumidores();
		display_info();
		PORTC->ISFR = (1 << 3);
	}
	if (PORTC->ISFR & (1 << 12)) {
		conmutar_productores();
		display_info();
		PORTC->ISFR = (1 << 12);
	}
}

void main_loop(void * pvParameters) {

	while (1) {
		delay(1000);
		display_info();
		delay(1000);
	}

}

int main(void) {

	queue_init();
	irclk_ini();
	lcd_ini();
	buttons_init();


	xTaskCreate(main_loop, "main_loop", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	//inicia FreeRTOS
	vTaskStartScheduler();

	while(1);

	return 0;

}

