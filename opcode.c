#include "opcode.h"
#include "chip8.h"
#include "display.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>

void (*chip8_table[16])(void) = 
{
	cpu_0, cpu_1, cpu_2, cpu_3, cpu_4, cpu_5, cpu_6, cpu_7, 
	cpu_arithmetic, cpu_9, cpu_a, cpu_b, cpu_c, cpu_d, cpu_e, cpu_f
};

//all opcodes that start with 0 are evaluated here
void cpu_0(void) {	
	/*printf("0 - %04x\n", opcode);*/
	unsigned short tail = opcode & 0x0FFF; 
	switch(tail) {
		case 0x00E0: //Clears the screen.
			for(int i = 0; i < (64 * 32); i++) {
				gfx[i] = 0;
			}
			draw_flag = 1;
			break;
		case 0x00EE: //returns from a subroutine.
			sp--;
			pc = stack[sp];
			break;
		default:
			printf("RCA is not implemented, exiting...\n");
			//exit(1);
			break;
	}
	pc += 2;
}

//Jumps to address NNN.
void cpu_1(void) {
	printf("1 - %04x\n", opcode);
	unsigned short NNN = opcode & 0x0FFF;

	pc = NNN;
}

//Calls subroutine at NNN.
void cpu_2(void) {
	printf("2 - %04x\n", opcode);
	unsigned short NNN = opcode & 0x0FFF;

	stack[sp] = pc;
	sp++;
	pc = NNN;
}

//Skips the next instruction if VX equals NN.
void cpu_3(void) {
	printf("3 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	if(V[x] == NN) {
		pc += 4;
	} else {
		pc += 2;
	}
}

//Skips the next instruction if VX doesn't equal NN.
void cpu_4(void) {
	printf("4 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	if(V[x] != NN) {
		pc += 4;
	} else {
		pc += 2;
	}
}

//Skips the next instruction if VX equals VY.
void cpu_5(void) {
	printf("5 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	if(V[x] == V[y]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

//Sets VX to NN
void cpu_6(void) {
	printf("6 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	V[x] = NN;
	pc += 2;
}

//Adds NN to VX. (Carry flag is not changed)
void cpu_7(void) {
	printf("7 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char NN = opcode & 0x00FF;

	V[x] += NN;
	pc += 2;
}

//all opcodes that start with 8 are evaluated here
void cpu_arithmetic(void) {
	printf("8 - %04x\n", opcode);
	int tail = opcode & 0x000F;
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
			if(V[y] > (0xFF - V[x])) {
				V[15] = 1;
			}	else {
				V[15] = 0;
			}
			V[x] += V[y];
			break;
		case(0x0005): //VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			if(V[y] > V[x]) {
				V[15] = 0;	
			} else {
				V[15] = 1;
			}
			V[x] -= V[y];
			break;
		case(0x0006): //Shifts VY right by one and copies the result to VX. VF is set to the value of the least significant bit of VY before the shift.
			V[15] = V[y] & 0x1; //get least significant bit
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
			V[15] = (V[y]) >> 3; //get most significant bit
			V[x] = V[y] << 1;
			break;
	}
	pc += 2;
}

//Skips the next instruction if VX doesn't equal VY. 
void cpu_9(void) {
	printf("9 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	if(V[x] != V[y]) {
		pc += 4;
	} else {
		pc += 2;
	}
}

//Sets I to the address NNN.
void cpu_a(void) {
	printf("10 - %04x\n", opcode);
	unsigned char NNN = opcode & 0x0FFF;

	I = NNN;
	pc += 2;
}

//Jumps to the address NNN plus V0.
void cpu_b(void) {
	printf("11 - %04x\n", opcode);
	unsigned char NNN = opcode & 0x0FFF;
	pc = NNN + V[0];
}

//Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
void cpu_c(void) {
	printf("12 - %04x\n", opcode);
	unsigned short x = (opcode & 0x0F00) >> 8;
	unsigned short NN = opcode & 0x00FF;
	unsigned short random_number = rand() % 256;

	V[x] = random_number & NN;	
	pc += 2;
}

/*Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen*/
void cpu_d(void) {
	printf("13 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned char N = opcode & 0x000F;
	unsigned short pixel;

	V[15] = 0;
	for (int yline = 0; yline < N; yline++)	{
		pixel = memory[I + yline];
		for(int xline = 0; xline < 8; xline++) {
			if((pixel & (0x80 >> xline)) != 0) {
				if(gfx[(V[x] + xline + ((V[y] + yline) * 64))] == 1) {
					V[15] = 1;
				}
				gfx[V[x] + xline + ((V[y] + yline) * 64)] ^= 1;
			}
		}
	}

	draw_flag = 1;
	pc += 2;
}

//all opcodes that start with e are evaluated here
void cpu_e(void) {
	printf("14 - %04x\n", opcode);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char tail = opcode & 0x00FF;

	switch(tail) {
		case 0x009E: //Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
			if(key[V[x]] != 0) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0x00A1: //Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
			if(key[V[x]] == 0) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
	}
}

//all opcodes that start with f are evaluated here
void cpu_f(void) {
	printf("15 - %04x\n", opcode);
	int tail = opcode & 0x00FF;
	unsigned char x = (opcode & 0x0F00) >> 8; 
	int key_pressed = 0;

	switch(tail) {
		case 0x0007: //Sets VX to the value of the delay timer.
			V[x] = delay_timer;
			break;
		case 0x000A:
			for(int i = 0; i < 16; i++) {
				if(key[i] != 0) {
					V[x] = i;
					key_pressed = 1;
				}
			}
			if(!key_pressed) {
				printf("rewind...\n");
				return;
			}
			break;
		case 0x0015: //Sets the delay timer to VX.
			delay_timer = V[x];
			break;
		case 0x0018: //Sets the sound timer to VX.
			sound_timer = V[x];
			break;
		case 0x001E: //Adds VX to I.
			I += V[x];
			break;
		case 0x0029: //Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			I = V[x] * 5;
			break;
		case 0x0033:
			memory[I] = V[x] / 100;
			memory[I + 1] = (V[x] / 10) % 10;
			memory[I + 2] = V[x] % 10;
			break;
		case 0x0055: //Stores V0 to VX (including VX) in memory starting at address I. I is increased by 1 for each value written.
			for(int i = 0; i <= x; i++) {
				memory[I + i] = V[i];
			}
			I += x + 1;
			break;
		case 0x0065: //Fills V0 to VX (including VX) with values from memory starting at address I. I is increased by 1 for each value written.
			for(int i = 0; i <= x; i++) {
				V[i] = memory[I + i];
			}
			I += x + 1;
			break;
	}
	pc += 2;
}
