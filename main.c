#include "display.h"
#include "chip8.h"
#include "opcode.h"
#include <stdio.h>

int main(void) {
	initialize_chip8();	
	create_window();
	emulate_cycle();
	destroy_window();
	return 0;
}	
