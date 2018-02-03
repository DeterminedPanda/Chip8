#include "SDL2/SDL.h"
#include "display.h"
#include <stdio.h>

#define WINDOW_WIDTH 1024	 
#define WINDOW_HEIGHT 512
#define WINDOW_NAME "Chip8 Emulator"
#define W 64
#define H 32

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

void create_window() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);
}

void draw(unsigned char gfx[]) { 
	printf("drawing\n");
	unsigned int pixels[64 * 32];

	for (int i = 0; i < 2048; ++i) {
		uint8_t pixel = gfx[i];
		pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
	}
	SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void destroy_window() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
