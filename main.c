#include <stdio.h>
#include "chip8.h"
#include "opcode.h"

int main(void) {
	printf("%d rofl\n", (0x8000 >> 15));
	printf("initializing chip8 emulator...");
	initialize_chip8();	
	load_rom();
	for(int i = 0; i < 100; i++) {
		emulate_cycle();
	}
	printf("done!\n");	return 0;
}	
