#include "display.h"
#include "chip8.h"
#include <stdio.h>

int main(void) {
	struct Chip8 chip;
	initialize_emulation(&chip);	
	create_window();
	emulate_cycle(&chip);
	destroy_window();

	return 0;
}	
