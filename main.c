#include <stdio.h>
#include "chip8.h"
#include "opcode.h"

int main(void) {
	printf("%04x rofl\n", (0xA2EA & 0xF000) >> 12);
	printf("initializing chip8 emulator...");
	initialize_chip8();	
	load_rom();
	for(int i = 0; i < 100; i++) {
		emulate_cycle();
	}
	printf("done!\n");	return 0;
}	
