#include "SDL2/SDL.h"
#include "display.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "Chip8 Emulator"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

void create_window() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
}

//TODO finish this function
void draw(unsigned char x, unsigned char y, unsigned int N) { 
	printf("drawing\n");
	for(int height = 0; height < N; height++) {
		for(int width = 0; width < 8; width++) {
			SDL_RenderDrawPoint(renderer, x + width, y + height);
		}
	}
	SDL_RenderPresent(renderer);
}

void clear_screen() {
	SDL_RenderClear(renderer);
}

void destroy_window() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
