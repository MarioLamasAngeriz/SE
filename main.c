#include "includes/MKL46Z4.h"
#include "drivers/lcd.h"

#define SIZE 10
// LED (RG)
// LED_GREEN = PTD5
// LED_RED = PTE29

void irclk_ini(){
	MCG->C1 = MCG_C1_IRCLKEN(1) | MCG_C1_IREFSTEN(1);
	MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
}

void delay (void) {
	volatile int i;
	for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
void led_green_init (void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
	PORTD->PCR[5] |= PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1 << 5); 
	GPIOD->PSOR |= (1 << 5);
}

// LED_RED = PTE29
void led_red_init (void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
	PORTE->PCR[29] |= PORT_PCR_MUX(1);
	GPIOE->PDDR |= (1 << 29);
	GPIOE->PSOR |= (1 << 29);
}

void led_green_toggle (void) {
	GPIOD->PTOR |= (1 << 5);
}

void led_red_toggle (void) {
	GPIOE->PTOR |= (1 << 29);
}

int led_green_detect (void) {
	return(GPIOD->PDIR & (1 << 5));
}

int led_red_detect (void) {
	return(GPIOE->PDIR & (1 << 29));
}

volatile uint8_t sequence[SIZE] = {0, 1, 0, 1, 0, 0, 0, 0, 1, 1};
volatile uint16_t indice = 0; //iteracion

volatile uint8_t botonIzq = 0; //variables q deben actualizar as interrupcions
volatile uint8_t botonDer = 1;

volatile uint16_t hit = 0;     //resultado
volatile uint16_t misses = 0;

void ledsOnOffSec (int led_green_state, int led_red_state) {

	for (indice; indice <= SIZE; indice++){

		led_green_state = led_green_detect();
		led_red_state = led_red_detect();

		if(!led_red_state) { led_red_toggle(); }
		if(!led_green_state) { led_green_toggle(); }

		delay();

		if (sequence[indice]) {
			led_red_toggle(); 
		} else {
			led_green_toggle();
		}

		if (botonIzq & !led_red_detect() || botonDer & !led_green_detect()){
			hit++;
		} else {
			misses++;
		}

		delay();

		lcd_display_time(hit, misses);
	}

	led_green_state = led_green_detect();
	led_red_state = led_red_detect();

	if(!led_red_state) { led_red_toggle(); }
	if(!led_green_state) { led_green_toggle(); }

}

// SW1 = PTC3
void button1_init(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
	PORTC->PCR[3] |= PORT_PCR_MUX(1);
	PORTC->PCR[3] |= PORT_PCR_PE(1);
	PORTC->PCR[3] |= PORT_PCR_PS(1);
	GPIOC->PDDR &= ~(1 << 3); // aseguramos q es input
}

// SW3 = PTC12
void button3_init(void) { 
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
	PORTC->PCR[12] |= PORT_PCR_MUX(1);
	PORTC->PCR[12] |= PORT_PCR_PE(1);
	PORTC->PCR[12] |= PORT_PCR_PS(1); 
	GPIOC->PDDR &= ~(1 << 12); // aseguramos q es input
}

//boton esquerdo led vermello ////////////////// boton dereito led verde

int main(void) {

	irclk_ini();
	lcd_ini();

	led_green_init();
	led_red_init();

	int led_green_state = 0; //declarar mais arriba e usar para comprobar interrupcions? ou innecesario?
	int led_red_state = 0;

	ledsOnOffSec(led_green_state, led_red_state);

	while(1){
		delay();
		lcd_display_time(0, 0);
		delay();
		lcd_display_time(hit, misses); //falta parpadeo
		delay();
	}
}
