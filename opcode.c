#include "opcode.h"
#include "chip8.h"
#include <stdio.h>

void (*chip8_table[16])(void) = 
{
	cpu_0, cpu_1, cpu_2, cpu_3, cpu_4, cpu_5, cpu_6, cpu_7, 
	cpu_arithmetic, cpu_9, cpu_a, cpu_b, cpu_c, cpu_d, cpu_e, cpu_f
};

//TODO name 
//all opcodes that start with 0 are evaluated here
void cpu_0(void) {

}

//TODO name
//Jumps to address NNN.
void cpu_1(void) {
	unsigned short NNN = opcode & 0x0FFF;
	pc = NNN;
}

//TODO name
//all opcodes that start with 2 are evaluated here
void cpu_2(void) {

}

//TODO name
//Skips the next instruction if VX equals NN.
void cpu_3(void) {
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	if(V[x] == NN) {
		pc += 4;
	} else {
		pc += 2;
	}
}

//TODO name
//Skips the next instruction if VX doesn't equal NN.
void cpu_4(void) {
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	if(V[x] != NN) {
		pc += 4;
	} else {
		pc += 2;
	}

}

//TODO name
//Skips the next instruction if VX equals VY.
void cpu_5(void) {
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	if(V[x] == V[y]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

//TODO name
//Sets VX to NN
void cpu_6(void) {
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	V[x] = NN;
	pc += 2;
}

//TODO name
//Adds NN to VX.
void cpu_7(void) {
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	V[x] += NN;
	pc += 2;
}

//all opcodes that start with 8 are evaluated here
void cpu_arithmetic(void) {
	int tail = opcode & 0x0000F;
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	switch(tail) {
		case(0x0000): //Sets VX to the value of VY. 
			V[x] = V[y];
			break;
		case(0x0001): //Sets VX to VX or VY. 
			V[x] = V[x] | V[y];
			break;
		case(0x0002): //Sets VX to VX and VY.	
			V[x] = V[x] & V[y];
			break;
		case(0x0003): //Sets VX to VX xor VY.
			V[x] = V[x] ^ V[y];
			break;
		case(0x0004): //Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			V[x] += V[y];
			break;
		case(0x0005): //VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			V[x] -= V[y];
			if(V[y] > V[x]) {
				V[15] = 0;	
			} else {
				V[15] = 1;
			}
			break;
		case(0x0006): //Shifts VY right by one and copies the result to VX. VF is set to the value of the least significant bit of VY before the shift.
			V[15] = V[y] & 1; //get least significant bit
			V[x] = V[y] >> 1;
			break;
		case(0x0007): //Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			if(V[x] > V[y]) {
				V[15] = 0;
			} else {
				V[15] = 1;
			}
			V[x] = V[y] - V[x];
			break;
		case(0x000E): //Shifts VY left by one and copies the result to VX. VF is set to the value of the most significant bit of VY before the shift 
			V[15] = (V[y] >> 15) & 1; //get most significant bit
			V[x] = V[y] >> 1;
			break;
	}
	pc += 2;
}

//TODO name
//Skips the next instruction if VX doesn't equal VY. 
void cpu_9(void) {
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	if(V[x] != V[y]) {
		pc += 4;
	} else {
		pc+= 2;
	}
}

//TODO name
//Sets I to the address NNN.
void cpu_a(void) {
	unsigned char NNN = opcode & 0x0FFF;

	I = NNN;
}

//TODO name
//Jumps to the address NNN plus V0.
void cpu_b(void) {
	unsigned char NNN = opcode & 0x0FFF;

	pc = V[0] + NNN;
}

//todo name
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
