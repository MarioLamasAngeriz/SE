#define CPU_MKL46Z256VLL4 1
#define BOARD_TPM_BASEADDR TPM0
#define BOARD_FIRST_TPM_CHANNEL 2U
#define BOARD_SECOND_TPM_CHANNEL 5U
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

#include "includes/fsl_debug_console.h"
#include "includes/board.h"
#include "includes/fsl_tpm.h"
#include "includes/pin_mux.h"
#include "includes/clock_config.h"
#include "includes/lcd.h"

void setup_pwm(void) {

	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam[2];

	#ifndef TPM_LED_ON_LEVEL  
		#define TPM_LED_ON_LEVEL kTPM_LowTrue 
	#endif    
    
	tpmParam[0].chnlNumber = (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL;	
	tpmParam[0].level = TPM_LED_ON_LEVEL;
	tpmParam[0].dutyCyclePercent = 0U;

	tpmParam[1].chnlNumber = (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL;
	tpmParam[1].level = TPM_LED_ON_LEVEL;
	tpmParam[1].dutyCyclePercent = 0U;

    	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
    	CLOCK_SetTpmClock(1U);

    	TPM_GetDefaultConfig(&tpmInfo);
    	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    	TPM_SetupPwm(BOARD_TPM_BASEADDR, tpmParam, 2U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
    	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

	//TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm, 1U);		// rojo, valores de 0 a 90 unsigned
	//TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL, kTPM_EdgeAlignedPwm, 1U); 		// verde, valores de 0 a 90 unsigned
	
}

int main(void) {

	lcd_ini();
	setup_pwm();
	
	lcd_set(0, 1);
	lcd_set(1, 2);
	lcd_set(2, 3);
	lcd_set(3, 4);

	while (1) {

	}
}
