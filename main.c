#include "includes/MKL46Z4.h"

// LED (RG)
// LED_GREEN = PTD5
// LED_RED = PTE29

void delay(void)
{
  volatile int i;

  for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
void led_green_init() {
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
  PORTD->PCR[5] |= PORT_PCR_MUX(1);
  //PORTD->PCR[5] |= PORT_PCR_PE(1);
  GPIOD->PDDR |= (1 << 5); 
  GPIOD->PSOR |= (1 << 5);
}

// LED_RED = PTE29
void led_red_init() {
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
  PORTE->PCR[29] |= PORT_PCR_MUX(1);
  //PORTE->PCR[29] |= PORT_PCR_PE(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR |= (1 << 29);
}

// SW1 = PTC3
void button1_init(){
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTE->PCR[3] |= PORT_PCR_MUX(1);
  PORTC->PCR[3] |= PORT_PCR_PE(1);
  PORTC->PCR[3] |= PORT_PCR_PS(1);
  GPIOC->PDDR &= ~(1 << 3);//aseguramos q es input
}

// SW3 = PTC12
void button3_init(){ 
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTC->PCR[12] |= PORT_PCR_MUX(1);
  PORTC->PCR[12] |= PORT_PCR_PE(1);
  PORTC->PCR[12] |= PORT_PCR_PS(1); 
  GPIOC->PDDR &= ~(1 << 12);//aseguramos q es input
}

int main(void) {
  led_green_init();
  led_red_init();

  while (1) {
    
  }

  return 0;
}
