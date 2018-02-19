#include "opcode.h"
#include <stdio.h>
#include "display.h"
#include <unistd.h>


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

void start_emulation(void) {
	struct Chip8 chip;
	initialize_chip(&chip);
	load_font(&chip);
	load_rom(&chip);
	emulate_cycle(&chip);	
}

void initialize_chip(struct Chip8 *chip) {
	chip->pc = ROM_SP;
	chip->opcode = ROM_SP;
	chip->I = 0;
	chip->sp = 0;
	chip->delay_timer = 0;
	chip->sound_timer = 0;
	chip->draw_flag = 0;

	for(int i = 0; i < (64 * 32); i++) {
		chip->gfx[i] = 0;
	}

	for(int i = 0; i < 16; i++) {
		chip->V[i] = 0;
		chip->stack[i] = 0;
		chip->keys[i] = 0;
	}
}

void load_font(struct Chip8 *chip) {
	for(int i = 0; i < 80; ++i) {
		chip->memory[i] = chip8_fontset[i]; 
	}
}

void load_rom(struct Chip8 *chip) {
	FILE *game = fopen("PONG", "rb");
	fread(chip->memory + ROM_SP, 1, MEMORY_SIZE - ROM_SP, game);

	/*for(int i = 0; i < MEMORY_SIZE; i++) {*/
		/*printf("%d - %d\n", i, chip->memory[i]);*/
	/*}*/

	/*chip->V[0] = 0x3f;*/
	/*chip->V[1] = 0x0c;*/
	/*chip->memory[0x200 + 0] = 0xD0;*/
	/*chip->memory[0x200 + 1] = 0x15;*/
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
	SDLK_y,
	SDLK_x,
	SDLK_c,
	SDLK_v
};


void emulate_cycle(struct Chip8 *chip) {
	while(1) {
		fetch_opcode(chip); //gets the next opcode
		chip8_table[(chip->opcode >> 12)](chip); //calls the opcodes corresponding function	

		if(chip->delay_timer > 0) {
			chip->delay_timer--;
		}

		if(chip->sound_timer > 0) {
			chip->sound_timer--;
		}

		// process sdl events
		SDL_Event e;
		while (SDL_PollEvent(&e)) {

			// process keydown events
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE)
					exit(0);


				for (int i = 0; i < 16; ++i) {
					if (e.key.keysym.sym == keymap[i]) {
						chip->keys[i] = 1;
					}
				}
			}
			// process keyup events
			if (e.type == SDL_KEYUP) {
				for (int i = 0; i < 16; ++i) {
					if (e.key.keysym.sym == keymap[i]) {
						chip->keys[i] = 0;
					}
				}
			}
		}

		if(chip->draw_flag == 1) {
			chip->draw_flag = 0;
			draw(chip->gfx);
		}

		SDL_Delay(1500);
	}
}

void fetch_opcode(struct Chip8 *chip) {
	chip->opcode = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
}
