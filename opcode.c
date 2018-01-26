#include "opcode.h"
#include "chip8.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>

void (*chip8_table[16])(void) = 
{
	cpu_0, cpu_1, cpu_2, cpu_3, cpu_4, cpu_5, cpu_6, cpu_7, 
	cpu_arithmetic, cpu_9, cpu_a, cpu_b, cpu_c, cpu_d, cpu_e, cpu_f
};

//TODO name 
//all opcodes that start with 0 are evaluated here
void cpu_0(void) {
	printf("0");
	unsigned short tail = opcode & 0x0FFF; 

	switch(tail) {
		case 0x00E0: //Clears the screen.
			clear_screen();
			pc += 2;
			break;
		case 0x00EE: //returns from a subroutine.
			sp--;
			pc = stack[sp];
			pc += 2;
			break;
	}
}

//TODO name
//Jumps to address NNN.
void cpu_1(void) {
	printf("1");
	unsigned short NNN = opcode & 0x0FFF;
	pc = NNN;
}

//TODO name
//Calls subroutine at NNN.
void cpu_2(void) {
	printf("2");
	sp++;
	stack[sp] = pc;
	unsigned short NNN = opcode & 0x0FFF;
	pc = NNN;
}

//TODO name
//Skips the next instruction if VX equals NN.
void cpu_3(void) {
	printf("3");
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
	printf("4");
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
	printf("5");
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
	printf("6");
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	V[x] = NN;
	pc += 2;
}

//TODO name
//Adds NN to VX.
void cpu_7(void) {
	printf("7");
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	V[x] += NN;
	pc += 2;
}

//all opcodes that start with 8 are evaluated here
void cpu_arithmetic(void) {
	printf("8");
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
	printf("9");
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
	printf("10");
	unsigned char NNN = opcode & 0x0FFF;

	I = NNN;
	pc += 2;
}

//TODO name
//Jumps to the address NNN plus V0.
void cpu_b(void) {
	printf("11");
	unsigned char NNN = opcode & 0x0FFF;

	pc = V[0] + NNN;
}

//TODO name
//Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
void cpu_c(void) {
	printf("12");
	unsigned short x = (opcode & 0x0F00) >> 8;
	unsigned short NN = opcode & 0x00FF;
	unsigned short random_number = rand() % 256;
	V[x] = random_number & NN;	
	pc += 2;
}

//TODO name
/*Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen*/
void cpu_d(void) {
	printf("13");
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned char N = opcode & 0x000F;

	draw(V[x], V[y], N);

	pc += 2;
}

//TODO name
//all opcodes that start with e are evaluated here
void cpu_e(void) {
	printf("14");
	pc += 2;
}

//TODO name
//all opcodes that start with f are evaluated here
void cpu_f(void) {
	printf("15");
	int tail = opcode & 0x00FF;
	unsigned char x = (opcode & 0x0F00) >> 8; 
	switch(tail) {
		case 0x0007:
			V[x] = delay_timer;
			break;
		case 0x000A:
			break;
		case 0x0015:
			delay_timer = V[x];
			break;
		case 0x0018:
			sound_timer = V[x];
			break;
		case 0x001E:
			I += V[x];
			break;
		case 0x0029:
			I = chip8_fontset[V[x]];
			break;
		case 0x0033:
			break;
		case 0x0055:
			break;
		case 0x0065:
			break;
	}
	pc += 2;
}
