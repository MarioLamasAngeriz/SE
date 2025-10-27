#define CPU_MKL46Z256VLL4 1
#include "includes/fsl_device_registers.h"
#include "includes/fsl_debug_console.h"

#include "includes/board.h"
#include "includes/pin_mux.h"

#define VALOR 4

unsigned int reverse_int(unsigned int in) {
	unsigned int out = 0;
	// Devolve o enteiro invertido bit a bit
	for (unsigned int i=0; i<32; i++) {
		out = out << 1;
		out |= in & 1;
		in = in >> 1;
	}
	return out;
}

int main() {
	char ch;

	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	PRINTF("start");

	while(1) {
		ch = GETCHAR();
		if (ch == '\n' || ch == '\r')
			break;
		PUTCHAR(ch);
	}

	PRINTF("end");

}
