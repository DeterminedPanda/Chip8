#include "display.h"
#include "chip8.h"
#include "opcode.h"
#include <stdio.h>

int main(void) {
	initialize_chip8();	
	create_window();
	for(;;) {
		emulate_cycle();
		printf(" - %04x ...\n", opcode);
	}

	destroy_window();
	return 0;
}	
