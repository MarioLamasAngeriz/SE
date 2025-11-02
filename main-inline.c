
#include "includes/MKL46Z4.h"
#include <stdint.h>
#define CPU_MKL46Z256VLL4 1
#include <stdlib.h>
#include "includes/fsl_debug_console.h"
#include "includes/board.h"
#include "includes/pin_mux.h"

unsigned int reverse_int(unsigned int in) {
	unsigned int out = 0;
	asm volatile (
		".syntax unified\n\t"
		"movs r3, %[input]\n\t"    // copiar parámetro a r3
		"movs %[output], #0\n\t"   // out = 0
		"movs r2, #32\n\t"         // contador = 32
        	"movs r4, #1\n\t"          // máscara 1
	    "1:\n\t"
        	"movs r1, r4\n\t"          
	        "lsls %[output], %[output], #1\n\t"
        	"ands r1, r3\n\t"
	        "orrs %[output], %[output], r1\n\t"
	        "lsrs r3, r3, #1\n\t"
	        "subs r2, r2, #1\n\t"
	        "bne 1b\n\t"
	        : [output] "=r" (out)         // salida
	        : [input] "r" (in)            // entrada
	        : "r1", "r2", "r3", "r4", "cc" // registros usados
    	);
	return out;
}


void imprimir_en_binario(unsigned int in) {
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
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
}

uint32_t systic_get(void) {
	return 0xFFFFFF - SysTick->VAL;
}

int main() {
	char num_str[10];
	char ch;
	int i = 0;
	uint32_t inicio;
	unsigned int reversed;
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
	
	unsigned int num = atoi(num_str);
	PRINTF("\r\nNumero a invertir bit a bit: %d\r\n", num);
	imprimir_en_binario(num);

	systic_conf();
	inicio = systic_get();
	
	reversed = reverse_int(num);
	
	fin = systic_get();
	ciclosCPU = fin - inicio; // realmente son ticks de systick pero como la cpu tiene la misma frecuencia que el systick son equivalentes

	PRINTF("Numero invertido bit a bit: %d\r\n", reversed);
	imprimir_en_binario(reversed);

	PRINTF("Número de ticks de SysTick (equivalentes a ciclos de CPU por tener ambos la misma frecuencia) \r\n"
			"para la ejecución de la función de inversion de bits = %d\r\n", ciclosCPU);
}
