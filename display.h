#include "SDL2/SDL.h"

void create_window(void);
void draw(unsigned char gfx[64][32]);
void destroy_window(void);

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
