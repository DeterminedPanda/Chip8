#include "chip8.h"
#include "opcode.h"
#include <stdio.h>
#include "display.h"
#include <unistd.h>

#define MEMORY_SIZE 4096
#define ROM_SP 0x200 //the rom starting point in memory

extern unsigned char memory[MEMORY_SIZE];
extern unsigned short opcode;
extern unsigned short pc;
extern unsigned char V[16];
extern unsigned short I;
extern unsigned short stack[16];
extern unsigned short sp;
extern unsigned char delay_timer;
extern unsigned char sound_timer;
extern unsigned char gfx[64 * 32];
extern unsigned int draw_flag;

unsigned char chip8_fontset[80] =
{ 
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initialize_chip8(void) {
	pc = ROM_SP;
	opcode = 0;
	I = 0;
	sp = 0;
	delay_timer = 0;
	sound_timer = 0;
	draw_flag = 0;
	load_font();
	load_rom();
}

void load_font(void) {
	for(int i = 0; i < 80; ++i) {
		memory[i] = chip8_fontset[i]; 
	}
}

void load_rom(void) {
	FILE *game = fopen("Pong.ch8", "rb");
	fread(memory + ROM_SP, 1, MEMORY_SIZE - ROM_SP, game);
	/*V[0] = 0;*/
	/*V[1] = 0;*/
	/*memory[0x200 + 0] = 0xD0;*/
	/*memory[0x200 + 1] = 0x15;*/
	/*memory[0x200 + 2] = 0xA0;*/
	/*memory[0x200 + 3] = 0x00;*/
	/*memory[0x200 + 4] = 0x61;*/
	/*memory[0x200 + 5] = 0x25;*/
	/*memory[0x200 + 6] = 0xD1;*/
	/*memory[0x200 + 7] = 0x25;*/
}

void emulate_cycle(void) {
	while(1) {
		fetch_opcode(); //gets the next opcode
		chip8_table[(opcode >> 12)](); //calls the opcodes corresponding function	

		if(delay_timer > 0) {
			delay_timer--;
		}

		if(sound_timer > 0) {
			sound_timer--;
		}

		if(draw_flag == 1) {
			draw_flag = 0;
			draw(gfx);
		}
	}
}

void fetch_opcode(void) {
	opcode = memory[pc] << 8 | memory[pc + 1];
}
