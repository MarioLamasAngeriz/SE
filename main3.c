#define CPU_MKL46Z256VLL4 1
#define TPM_MAX_VALUE 0xFFFF

#include <stdint.h>
#include <stdlib.h>
#include "includes/MKL46Z4.h"
#include "includes/fsl_debug_console.h"
#include "includes/board.h"
#include "includes/pin_mux.h"


extern uint32_t reverse_bits(uint32_t in);

void imprimir_en_binario(uint32_t in) {
	PRINTF("Número en binario: 0b");
	for (int i = 31; i >= 0; i--) {
		if (in & (1u << i)) {
			PRINTF("1");
		} else { 
			PRINTF("0");
		}
		if (i % 8 == 0)
			PRINTF(" ");
	}
	PRINTF("\r\n");
}

void tmp0_conf(void) {
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0b01);
	TPM0->SC = 0;
	TPM0->SC = TPM_SC_CMOD(0b01) | TPM_SC_PS(0b000);
	TPM0->MOD = TPM_MAX_VALUE;
}

uint32_t tmp0_get(void) {
	return TPM0->CNT;
}

int main(void) {
	char num_str[10];
	char ch;
	int i = 0;
	uint32_t inicio;
	uint32_t reversed;
	uint32_t fin;
	uint32_t ciclosCPU;

	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	PRINTF("Introduce un número enteiro, max 9 dixitos\r\n");

	while (i < 9) {
		ch = GETCHAR();
		PUTCHAR(ch);
		if (ch == '\n' || ch == '\r')
			break;
		num_str[i++] = ch;
	}

	num_str[i] = '\0';
	
	uint32_t num = atoi(num_str);
	PRINTF("\r\nNúmero a invertir bit a bit: %u\r\n", num);
	imprimir_en_binario(num);

	tmp0_conf();
	inicio = tmp0_get();
	
	reversed = reverse_bits(num);
	
	fin = tmp0_get();
		
	if (fin < inicio) { //overflow?
        	ciclosCPU = (TPM_MAX_VALUE - inicio) + fin + 1;
    	} else {
        	ciclosCPU = fin - inicio;
    	}

	PRINTF("Número invertido bit a bit: %u\r\n", reversed);
	imprimir_en_binario(reversed);

	PRINTF("Número de ciclos de CPU para a execución de la función de inversion de bits = %u\r\n", ciclosCPU);
}
