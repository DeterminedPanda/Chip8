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
extern unsigned char keys[16];

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
}

unsigned char keymap[16] = {
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_r,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_f,
	SDLK_z,
	SDLK_x,
	SDLK_c,
	SDLK_v
};


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

		// Process SDL events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {

            // Process keydown events
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);


                for (int i = 0; i < 16; ++i) {
                    if (e.key.keysym.sym == keymap[i]) {
                        key[i] = 1;
                    }
                }
            }
            // Process keyup events
            if (e.type == SDL_KEYUP) {
                for (int i = 0; i < 16; ++i) {
                    if (e.key.keysym.sym == keymap[i]) {
                        key[i] = 0;
                    }
                }
            }
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
