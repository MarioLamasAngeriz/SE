#include "includes/MKL46Z4.h"
#include <stdint.h>
#define CPU_MKL46Z256VLL4 1
#include <stdlib.h>
#include "includes/fsl_debug_console.h"
#include "includes/board.h"
#include "includes/pin_mux.h"

uint32_t reverse_int(uint32_t in) {
	uint32_t out = 0;
	for (uint32_t i=0; i<32; i++) {
		out = out << 1;
		out |= in & 1;
		in = in >> 1;
	}
	return out;
}

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

void systic_conf(void) {
	SysTick->CTRL = 0;
	SysTick->LOAD = 0xFFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}

uint32_t systic_get(void) {
	return SysTick->VAL;
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
	PRINTF("\r\nNumero a invertir bit a bit: %u\r\n", num);
	imprimir_en_binario(num);

	systic_conf();
	inicio = systic_get();
	
	reversed = reverse_int(num);
	
	fin = systic_get();
	ciclosCPU = (inicio - fin) & 0xFFFFFF; // realmente son ticks de systick pero como la cpu tiene la misma frecuencia que el systick son equivalentes

	PRINTF("Numero invertido bit a bit: %u\r\n", reversed);
	imprimir_en_binario(reversed);

	PRINTF("Número de ticks de SysTick (equivalentes a ciclos de CPU por tener ambos la misma frecuencia) \r\n"
			"para la ejecución de la función de inversion de bits = %u\r\n", ciclosCPU);
}
