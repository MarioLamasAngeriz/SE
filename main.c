#include "includes/MKL46Z4.h"
#include <strings.h>
#define SIZE 10
// LED (RG)
// LED_GREEN = PTD5
// LED_RED = PTE29

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

volatile unsigned int sequence[SIZE] = {0, 1, 0, 1, 0, 0, 0, 0, 1, 1};
volatile unsigned int indice = 0;

void ledsOnOffSec (int led_green_state, int led_red_state) {
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

    indice == SIZE ? indice = 0 : indice++;
    delay();
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
  
  led_green_init();
  led_red_init();

  int led_green_state = 0;
  int led_red_state = 0;

  while (1) {
	  ledsOnOffSec(led_green_state, led_red_state);
  }

  return 0;
}
