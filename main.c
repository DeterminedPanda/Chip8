#include "display.h"
#include "chip8.h"
#include "opcode.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

int main(void) {
	initialize_chip8();	
	int window_created = initialize_window();
	if(window_created == FALSE) {
		return FALSE; 
	}
	for(int i = 0; i < 10; i++) {
		emulate_cycle();
		printf(" - %04x ...\n", opcode);
	}
	return TRUE;
}	
