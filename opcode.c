#include "opcode.h"
#include "chip8.h"
#include <stdio.h>

void execute_opcode(void) {
	chip8_table[(opcode >> 12)]();
}

void (*chip8_table[17])(void) = 
{
	cpu_0      , cpu_1, cpu_2, cpu_3, cpu_4, cpu_5, cpu_6, cpu_7, 
	cpu_arithmetic, cpu_9, cpu_a, cpu_b, cpu_c, cpu_d, cpu_e, cpu_f,
	cpu_null
};

void cpu_null(void) {
	printf("wtf %04x\n", opcode);
}

//TODO name 
//all opcodes that start with 0 are evaluated here
void cpu_0(void) {

}

//TODO name
//all opcodes that start with 1 are evaluated here
void cpu_1(void) {

}

//TODO name
//all opcodes that start with 2 are evaluated here
void cpu_2(void) {

}

//TODO name
//all opcodes that start with 3 are evaluated here
void cpu_3(void) {

}

//TODO name
//all opcodes that start with 4 are evaluated here
void cpu_4(void) {

}

//TODO name
//all opcodes that start with 5 are evaluated here
void cpu_5(void) {

}

//TODO name
//all opcodes that start with 6 are evaluated here
void cpu_6(void) {

}

//TODO name
//all opcodes that start with 7 are evaluated here
void cpu_7(void) {

}

//all opcodes that start with 8 are evaluated here
void cpu_arithmetic(void) {
	int tail = opcode & 0x0000F;
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	switch(tail) {
		case(0x0000):
			V[x] = V[y];
			break;
		case(0x0001):
			V[x] = V[x] | V[y];
			break;
		case(0x0002):
			V[x] = V[x] & V[y];
			break;
		case(0x0003):
			V[x] = V[x] ^ V[y];
			break;
		case(0x0004):
			V[x] += V[y];
			break;
		case(0x0005):
			//possible FIXME, should V[x] only be subtracted by V[y]
			//when V[y] is bigger than V[X]?
			V[x] -= V[y];
			if(y > x) {
				V[15] = 0;	
			} else {
				V[15] = 1;
			}
			break;
		case(0x0006):
			V[15] = V[y] & 1; //get least significant bit
			V[x] = V[y] >> 1;
			break;
		case(0x0007):
			//possible FIXME, should V[y] only be subtracted by V[x]
			//when V[x] is bigger than V[y]?
			if(x > y) {
				V[15] = 0;
			} else {
				V[15] = 1;
			}
			V[x] = V[y] - V[x];
			break;
		case(0x000E): 
			V[15] = (V[y] >> 15) & 1; //get most significant bit
			V[x] = V[y] >> 1;
			break;
	}
}

//TODO name
//all opcodes that start with 9 are evaluated here
void cpu_9(void) {

}

//TODO name
//all opcodes that start with a are evaluated here
void cpu_a(void) {

}

//TODO name
//all opcodes that start with b are evaluated here
void cpu_b(void) {

}

//TODO name
//all opcodes that start with c are evaluated here
void cpu_c(void) {

}

//TODO name
//all opcodes that start with d are evaluated here
void cpu_d(void) {

}

//TODO name
//all opcodes that start with e are evaluated here
void cpu_e(void) {

}

//TODO name
//all opcodes that start with f are evaluated here
void cpu_f(void) {

}
