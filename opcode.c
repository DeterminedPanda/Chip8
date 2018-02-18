#include "opcode.h"
#include "display.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>

void (*chip8_table[16])() = 
{
	cpu_0, cpu_1, cpu_2, cpu_3, cpu_4, cpu_5, cpu_6, cpu_7, 
	cpu_8, cpu_9, cpu_a, cpu_b, cpu_c, cpu_d, cpu_e, cpu_f
};

//all chip->opcodes that start with 0 are evaluated here
void cpu_0(struct Chip8 *chip) {	
	printf("0 - %04x\n", chip->opcode);
	unsigned short tail = chip->opcode & 0x0FFF; 

	switch(tail) {
		case 0x00E0: //Clears the screen.
			for(int i = 0; i < (64 * 32); i++) {
				chip->gfx[i] = 0;
			}
			chip->draw_flag = 1;
			break;
		case 0x00EE: //returns from a subroutine.
			chip->sp--;
			chip->pc = chip->stack[chip->sp];
			break;
		default:
			printf("RCA is not implemented, exiting...\n");
			/*exit(1);*/
			break;
	}
	chip->pc += 2;
	//TODO muss das hier sein?
}

//Jumps to address NNN.
void cpu_1(struct Chip8 *chip) {
	printf("1 - %04x\n", chip->opcode);
	unsigned short NNN = chip->opcode & 0x0FFF;

	chip->pc = NNN;
}

//Calls subroutine at NNN.
void cpu_2(struct Chip8 *chip) {
	printf("2 - %04x\n", chip->opcode);
	unsigned short NNN = chip->opcode & 0x0FFF;

	chip->stack[chip->sp] = chip->pc;
	chip->sp++;
	chip->pc = NNN;
}

//Skips the next instruction if chip->VX equals NN.
void cpu_3(struct Chip8 *chip) {
	printf("3 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char NN = chip->opcode & 0x00FF;

	if(chip->V[x] == NN) {
		chip->pc += 4;
	} else {
		chip->pc += 2;
	}
}

//Skips the next instruction if chip->VX doesn't equal NN.
void cpu_4(struct Chip8 *chip) {
	printf("4 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char NN = chip->opcode & 0x00FF;

	if(chip->V[x] != NN) {
		chip->pc += 4;
	} else {
		chip->pc += 2;
	}
}

//Skips the next instruction if chip->VX equals chip->VY.
void cpu_5(struct Chip8 *chip) {
	printf("5 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char y = (chip->opcode & 0x00F0) >> 4;

	if(chip->V[x] == chip->V[y]) {
		chip->pc += 4;
	} else {
		chip->pc += 2;
	}
}

//Sets chip->VX to NN
void cpu_6(struct Chip8 *chip) {
	printf("6 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char NN = chip->opcode & 0x00FF;

	chip->V[x] = NN;
	chip->pc += 2;
}

//Adds NN to chip->VX. (Carry flag is not changed)
void cpu_7(struct Chip8 *chip) {
	printf("7 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char NN = chip->opcode & 0x00FF;

	chip->V[x] += NN;
	chip->pc += 2;
}

//all chip->opcodes that start with 8 are evaluated here
void cpu_8(struct Chip8 *chip) {
	printf("8 - %04x\n", chip->opcode);
	int tail = chip->opcode & 0x000F;
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char y = (chip->opcode & 0x00F0) >> 4;

	switch(tail) {
		case(0x0000): //Sets chip->VX to the value of chip->VY. 
			chip->V[x] = chip->V[y];
			break;
		case(0x0001): //Sets chip->VX to chip->VX or chip->VY. 
			chip->V[x] = chip->V[x] | chip->V[y];
			break;
		case(0x0002): //Sets chip->VX to chip->VX and chip->VY.	
			chip->V[x] = chip->V[x] & chip->V[y];
			break;
		case(0x0003): //Sets chip->VX to chip->VX xor chip->VY.
			chip->V[x] = chip->V[x] ^ chip->V[y];
			break;
		case(0x0004): //Adds chip->VY to chip->VX. chip->VF is set to 1 when there's a carry, and to 0 when there isn't.
			chip->V[x] = chip->V[x] + chip->V[y];
			if(chip->V[y] > (255 - chip->V[x])) {
				chip->V[15] = 1;
			} else {
				chip->V[15] = 0;
			}
			break;
		case(0x0005): //chip->VY is subtracted from chip->VX. chip->VF is set to 0 when there's a borrow, and 1 when there isn't.
			if(chip->V[y] > chip->V[x]) {
				chip->V[15] = 0;
			}
			else {
				chip->V[15] = 1;
			}
			chip->V[x] = chip->V[x] - chip->V[y];
			break;
		case(0x0006): //Shifts chip->VY right by one and copies the result to chip->VX. chip->VF is set to the value of the least significant bit of chip->VY before the shift.
			chip->V[15] = chip->V[y] & 0x1; //get least significant bit
			chip->V[x] = chip->V[y] >> 1;
			break;
		case(0x0007): //Sets chip->VX to chip->VY minus chip->VX. chip->VF is set to 0 when there's a borrow, and 1 when there isn't.
			if(chip->V[x] > chip->V[y]) {
				chip->V[15] = 0;
			} else {
				chip->V[15] = 1;
			}
			chip->V[x] = chip->V[y] - chip->V[x];
			break;
		case(0x000E): //Shifts chip->VY left by one and copies the result to chip->VX. chip->VF is set to the value of the most significant bit of chip->VY before the shift 
			chip->V[15] = chip->V[y] >> 3; //get most significant bit
			chip->V[x] = chip->V[y] << 1;
			break;
	}
	chip->pc += 2;
}

//Skips the next instruction if chip->VX doesn't equal chip->VY. 
void cpu_9(struct Chip8 *chip) {
	printf("9 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char y = (chip->opcode & 0x00F0) >> 4;

	if(chip->V[x] != chip->V[y]) {
		chip->pc += 4;
	} else {
		chip->pc += 2;
	}
}

//Sets chip->I to the address NNN.
void cpu_a(struct Chip8 *chip) {
	printf("10 - %04x\n", chip->opcode);
	unsigned char NNN = chip->opcode & 0x0FFF;

	chip->I = NNN;
	chip->pc += 2;
}

//Jumps to the address NNN plus chip->V0.
void cpu_b(struct Chip8 *chip) {
	printf("11 - %04x\n", chip->opcode);
	unsigned char NNN = chip->opcode & 0x0FFF;
	chip->pc = NNN + chip->V[0];
}

//Sets chip->VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
void cpu_c(struct Chip8 *chip) {
	printf("12 - %04x\n", chip->opcode);
	unsigned short x = (chip->opcode & 0x0F00) >> 8;
	unsigned short NN = chip->opcode & 0x00FF;
	unsigned short random_number = rand() % 256;

	chip->V[x] = random_number & NN;	
	chip->pc += 2;
}

/*Draws a chip->sprite at coordinate (chip->VX, chip->VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from chip->memory location chip->I; I value doesn’t change after the execution of this instruction. As described above, chip->VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen*/
void cpu_d(struct Chip8 *chip) {
	printf("13 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char y = (chip->opcode & 0x00F0) >> 4;
	unsigned char N = chip->opcode & 0x000F;
	unsigned short pixel;

	chip->V[15] = 0;
	for (int yline = 0; yline < N; yline++)	{
		pixel = chip->memory[chip->I + yline];
		for(int xline = 0; xline < 8; xline++) {
			if((pixel & (0x80 >> xline)) != 0) {
				if(chip->gfx[(chip->V[x] + xline + ((chip->V[y] + yline) * 64))] == 1) {
					chip->V[15] = 1;
				}
				chip->gfx[chip->V[x] + xline + ((chip->V[y] + yline) * 64)] ^= 1;
			}
		}
	}

	chip->draw_flag = 1;
	chip->pc += 2;
}

//all chip->opcodes that start with e are evaluated here
void cpu_e(struct Chip8 *chip) {
	printf("14 - %04x\n", chip->opcode);
	unsigned char x = (chip->opcode & 0x0F00) >> 8;
	unsigned char tail = chip->opcode & 0x00FF;

	switch(tail) {
		case 0x009E: //Skips the next instruction if the chip->keys stored in chip->VX is pressed. (Usually the next instruction is a jump to skip a code block)
			if(chip->keys[chip->V[x]] != 0) {
				chip->pc += 4;
			} else {
				chip->pc += 2;
			}
			break;
		case 0x00A1: //Skips the next instruction if the chip->keys stored in chip->VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
			if(chip->keys[chip->V[x]] == 0) {
				chip->pc += 4;
			}
			else {
				chip->pc += 2;
			}
			break;
	}
}

//all chip->opcodes that start with f are evaluated here
void cpu_f(struct Chip8 *chip) {
	printf("15 - %04x\n", chip->opcode);
	int tail = chip->opcode & 0x00FF;
	unsigned char x = (chip->opcode & 0x0F00) >> 8; 
	int key_pressed = 0;

	switch(tail) {
		case 0x0007: //Sets chip->VX to the value of the delay timer.
			chip->V[x] = chip->delay_timer;
			break;
		case 0x000A:
			for(int i = 0; i < 16; i++) {
				if(chip->keys[i] != 0) {
					chip->V[x] = i;
					key_pressed = 1;
				}
			}
			if(!key_pressed) {
				printf("rewind...\n");
				return;
			}
			break;
		case 0x0015: //Sets the delay timer to chip->VX.
			chip->delay_timer = chip->V[x];
			break;
		case 0x0018: //Sets the sound timer to chip->VX.
			chip->sound_timer = chip->V[x];
			break;
		case 0x001E: //Adds chip->VX to chip->I.
			chip->I += chip->V[x];
			break;
		case 0x0029: //Sets chip->I to the location of the chip->sprite for the character in chip->VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			chip->I = chip->V[x] * 5;
			break;
		case 0x0033:
			chip->memory[chip->I] = chip->V[x] / 100;
			chip->memory[chip->I + 1] = (chip->V[x] / 10) % 10;
			chip->memory[chip->I + 2] = chip->V[x] % 10;
			break;
		case 0x0055: //Stores chip->V0 to chip->VX (including chip->VX) in chip->memory starting at address chip->I. I is increased by 1 for each value written.
			for(int i = 0; i <= x; i++) {
				chip->memory[chip->I + i] = chip->V[i];
			}
			chip->I += x + 1;
			break;
		case 0x0065: //Fills chip->V0 to chip->VX (including chip->VX) with values from chip->memory starting at address chip->I. I is increased by 1 for each value written.
			for(int i = 0; i <= x; i++) {
				chip->V[i] = chip->memory[chip->I + i];
			}
			chip->I += x + 1;
			break;
	}
	chip->pc += 2;
}
