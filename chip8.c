#include "chip8.h"
#include "opcode.h"
#include <stdio.h>

#define MEMORY_SIZE 4096
#define ROM_SP 0x200 //the rom starting point in memory

unsigned char memory[MEMORY_SIZE];
unsigned short opcode;
unsigned short pc;

void initialize_chip8(void) {
	pc = ROM_SP;
}

void load_rom(void) {
	FILE *fp = fopen("Pong.ch8", "r");
	fread(memory + ROM_SP, 1, MEMORY_SIZE - ROM_SP, fp);
}

void emulate_cycle(void) {
	fetch_opcode();
	execute_opcode();		
}

void fetch_opcode(void) {
	opcode = memory[pc] << 8 | memory[pc + 1];
	pc += 2;
}
