#include "opcode.h"
#include "chip8.h"
#include <stdio.h>

void execute_opcode(void) {
	chip8_table[(opcode >> 12)]();
}

void (*chip8_table[17])(void) = 
{
	cpuNULL      , cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, 
	cpuARITHMETIC, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
	cpuNULL
};

void cpuNULL(void) {
	//not implemented yet
}

//all opcodes that start with 8 are evaluated here
void cpuARITHMETIC(void) {
	int roflmao = opcode & 0x0000F;
	printf("%04x %04x\n", opcode, roflmao);
}
