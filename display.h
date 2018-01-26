#include "SDL2/SDL.h"

void create_window(void);
void draw(unsigned char x, unsigned char y, unsigned int N);
void clear_screen(void);
void destroy_window(void);

SDL_Window *window;
SDL_Renderer *renderer;
