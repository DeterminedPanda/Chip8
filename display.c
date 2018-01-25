#include "SDL2/SDL.h"
#include "display.h"
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_NAME "Chip8 Emulator"
#define TRUE 1
#define FALSE 0

int initialize_window() {
	SDL_Window *window;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		return FALSE;
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return TRUE;
}
