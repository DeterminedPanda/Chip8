#include "chip8.h"
#include <stdio.h>

#define MEMORY_SIZE 4096
#define ROM_SP 0x200 //the rom starting point in memory variable

unsigned char memory[MEMORY_SIZE];
unsigned short opcode;
unsigned short pc;

void initialize_chip8() {
	pc = ROM_SP;
	load_rom();
	for(int i = 0; i < 10 / 2; i++) {
		emulate_cycle();
		printf("%04x lol\n", opcode);
	}
}

void load_rom() {
	FILE *fp = fopen("Pong.ch8", "r");
	fread(memory + ROM_SP, 1, MEMORY_SIZE - ROM_SP, fp);
}

void emulate_cycle() {
	fetch_opcode();
}

void fetch_opcode() {
	opcode = memory[pc] << 8 | memory[pc + 1];
	pc += 2;
}
