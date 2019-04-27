#ifndef __PIXELRENDERER_H_
#define __PIXELRENDERER_H_
#include <stdbool.h>
#include <SDL2/SDL.h>
bool init_pixelrenderer(SDL_Renderer* renderer, SDL_Window* window, int width, int height, float max_depth);
void shutdown_pixelrenderer();
void start_pixelrenderer();
void end_pixelrenderer();
bool write_pixel(int x, int y, Uint32 colour, float depth);
void force_write(int x, int y, Uint32 colour, float depth);

Uint32 set_colour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void get_colours(Uint32 colour, Uint8* r, Uint8* g, Uint8* b, Uint8* a);
#endif

