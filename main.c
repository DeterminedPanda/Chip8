#include "display.h"
#include "chip8.h"
#include <stdio.h>

int main(void) {
	create_window();
	start_emulation();	
	destroy_window();
	return 0;
}	
