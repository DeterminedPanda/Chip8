#include <stdio.h>
#include "chip8.h"
#include "opcode.h"

int main(void) {
	printf("initializing chip8 emulator...\n");
	initialize_chip8();	
	for(int i = 0; i < 100; i++) {
		emulate_cycle();
	}
}	
