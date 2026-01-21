#define BOARD_TPM_BASEADDR TPM0
#define BOARD_FIRST_TPM_CHANNEL 2U
#define BOARD_SECOND_TPM_CHANNEL 5U
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

#include "includes/MKL46Z4.h"
#include "drivers/fsl_tpm.h"
#include "drivers/fsl_debug_console.h"
#include "board.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include <stdint.h>

void setup_pwm(void) {
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];

	tpmParam[0].chnlNumber = (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL;	
	tpmParam[0].level = kTPM_LowTrue ;
	tpmParam[0].dutyCyclePercent = 0U;

	tpmParam[1].chnlNumber = (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL;
	tpmParam[1].level = kTPM_LowTrue ;
	tpmParam[1].dutyCyclePercent = 0U;

	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

	TPM_SetupPwm(BOARD_TPM_BASEADDR, tpmParam, 2U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
}

void change_leds(uint8_t rojo, uint8_t verde) {
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm, rojo);		// rojo, valores de 0 a 100 unsigned
	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL, kTPM_EdgeAlignedPwm, verde); 		// verde, valores de 0 a 100 unsigned
}

void adc0_init(void) {
	ADC0->CFG1 = ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1);
	ADC0->CFG2 = 0x00;
	ADC0->SC2 = 0x00;
	ADC0->SC3 = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(2);
}

uint16_t adc0_leer_luz(void) {
	ADC0->SC1[0] = ADC_SC1_ADCH(3);
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	return (uint16_t)ADC0->R[0];
}

void Port_Init(void){
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK | SIM_SCGC6_TPM0_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
}

int main(void) {

    	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	CLOCK_SetTpmClock(1U);

	Port_Init();
	adc0_init();
	setup_pwm();

	while(1) {
		uint16_t lectura = adc0_leer_luz();
		PRINTF("Lectura ADC: %d \r\n", lectura);

		float verde, rojo;

		verde = (lectura / 4095.0f) * 100.0f;
		rojo = 100.0f - verde;

		change_leds(rojo, verde);
		for (volatile int i = 0; i < 500000; i++); 
	}
}


