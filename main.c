#include "includes/MKL46Z4.h"
#include "drivers/lcd.h"
#include <stdint.h>

#define SIZE 32

volatile uint8_t sequence[SIZE] = {0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1};
volatile uint16_t indice = 0; //iteracion

volatile uint8_t botonIzq = 0; //variables q deben actualizar as interrupcions
volatile uint8_t botonDer = 0;

volatile uint8_t botonPress = 0;
volatile uint8_t end = 0;

volatile uint16_t hit = 0;     //resultado
volatile uint16_t misses = 0;

void irclk_ini (void){
	MCG->C1 = MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
	MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
}

void delay (void) {
	volatile int i;
	for (i = 0; i < 1000000; i++);
}

void encender_led_green (void) {
	GPIOD->PCOR = (1 << 5);
}

void encender_led_red (void) {
	GPIOE->PCOR = (1 << 29);
}

void apagar_led_green (void) {
	GPIOD->PSOR = (1 << 5);
}

void apagar_led_red (void) {
	GPIOE->PSOR = (1 << 29);
}

// LED_GREEN = PTD5
void led_green_init (void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD->PCR[5] |= PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1 << 5); 
	apagar_led_green();
}

// LED_RED = PTE29
void led_red_init (void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[29] |= PORT_PCR_MUX(1);
	GPIOE->PDDR |= (1 << 29);
	apagar_led_red();
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
	if (!end && !botonPress) {
		if (PORTC->ISFR & (1 << 3)) { //detecter flag de interrupcion 
			botonPress = 1;
			botonDer = 1;
			PORTC->ISFR = (1 << 3);
		}
		if (PORTC->ISFR & (1 << 12)) {	//detectar flag de interrupcion 
			botonPress = 1;
			botonIzq = 1;
			PORTC->ISFR = (1 << 12);
		}
	} else {
		PORTC->ISFR = 0xFFFFFFFF; //borrando flags
	}
}

void ledsOnOffSec (void) {

	for (indice = 0; indice < SIZE; indice++){

		apagar_led_green();
		apagar_led_red();
		
		delay();

		if (sequence[indice]) {
			encender_led_red();
		} else {
			encender_led_green();
		}
	
		botonPress = 0;
		botonIzq = 0;
		botonDer = 0;

		delay();
	//	delay();

		if (botonPress) {
			if ((botonIzq && sequence[indice] == 1) || (botonDer && sequence[indice] == 0)) {
				hit++;
			} else {
				misses++;
			}
		} else {
			misses++;
		}

		apagar_led_green();
		apagar_led_red();
		
		lcd_display_time(hit, misses);

		delay();
	}
}

//boton esquerdo led vermello ////////////////// boton dereito led verde

int main(void) {

	irclk_ini();
	lcd_ini();

	led_green_init();
	led_red_init();
	buttons_init();
	
	lcd_display_time(hit, misses);
	ledsOnOffSec();

	end = 1;

	while(1){
		lcd_display_time(hit, misses);
		delay();
		LCD->GCR &= ~LCD_GCR_LCDEN_MASK;
		delay();
		LCD->GCR |= LCD_GCR_LCDEN_MASK;   
		delay();
	}
}
