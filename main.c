#include "includes/MKL46Z4.h"

// LED (RG)
// LED_GREEN = PTD5
// LED_RED = PTE29

void delay(void) {
  volatile int i;
  for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
void led_green_init(void) {
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
  PORTD->PCR[5] |= PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5); 
  GPIOD->PSOR |= (1 << 5);
}

// LED_RED = PTE29
void led_red_init(void) {
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
  PORTE->PCR[29] |= PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR |= (1 << 29);
}

void led_green_toggle(void) {
  GPIOD->PTOR |= (1 << 5);
}

void led_red_toggle(void) {
  GPIOE->PTOR |= (1 << 29);
}

// SW1 = PTC3
void button1_init(void) {
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTC->PCR[3] |= PORT_PCR_MUX(1);
  PORTC->PCR[3] |= PORT_PCR_PE(1);
  PORTC->PCR[3] |= PORT_PCR_PS(1);
  GPIOC->PDDR &= ~(1 << 3); // aseguramos q es input
}

// SW3 = PTC12
void button3_init(void) { 
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTC->PCR[12] |= PORT_PCR_MUX(1);
  PORTC->PCR[12] |= PORT_PCR_PE(1);
  PORTC->PCR[12] |= PORT_PCR_PS(1); 
  GPIOC->PDDR &= ~(1 << 12); // aseguramos q es input
}

int button1_detect(void) {
  return!(GPIOC->PDIR & (1 << 3)); // comprobamos nivel lógico, 1 -> sin presionar, 0 -> presionado  
}

int button3_detect(void){
  return!(GPIOC->PDIR & (1 << 12)); // comprobamos nivel lógico, 1 -> sin presionar, 0 -> presionadoo
}

int main(void) {

  int b1_prev = 0;
  int b3_prev = 0;

  led_green_init();
  led_red_init();
  button1_init();
  button3_init();

  while (1) {
    int b1 = button1_detect();
    int b3 = button3_detect();

    if (b1 && !b1_prev) {
      led_green_toggle();
    }

    if (b3 && !b3_prev) {
      led_red_toggle();
    }

    b1_prev = b1;
    b3_prev = b3;

    //delay();
  }

  return 0;
}
