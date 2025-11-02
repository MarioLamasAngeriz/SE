#define CPU_MKL46Z256VLL4 1
#include <stdlib.h>
#include "includes/fsl_debug_console.h"

#include "includes/board.h"
#include "includes/pin_mux.h"

unsigned int reverse_int(unsigned int in) {
	unsigned int out = 0;
	for (unsigned int i=0; i<32; i++) {
		out = out << 1;
		out |= in & 1;
		in = in >> 1;
	}
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

int main() {
	char num_str[10];
	char ch;
	int i = 0;

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

	unsigned int reversed = reverse_int(num);
	PRINTF("Numero invertido bit a bit: %d\r\n", reversed);
	imprimir_en_binario(reversed);
}
