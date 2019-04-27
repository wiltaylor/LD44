#ifndef __FONTRENDER_H_
#define __FONTRENDER_H_
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"

bool init_font(SDL_Renderer* rend);
void shutdown_font();

TTF_Font* load_font(char* path, int size);
void unload_font(TTF_Font* font);
Texture* render_text(char* text, Uint32 colour, TTF_Font* font);

void draw_text(int x, int y, char* text, Uint32 colour, TTF_Font* font);
#endif
