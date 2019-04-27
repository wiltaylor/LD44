#ifndef __TEXTURE_H_
#define __TEXTURE_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
    SDL_Texture* texture;
    Uint32* pixels;
    int pitch;
    Uint32 width;
    Uint32 height;
    SDL_PixelFormat* format;
    SDL_Rect rect;

}Texture;

bool init_texture(SDL_Window* window, SDL_Renderer* renderer);
void shutdown_texture();

Texture* load_texture(char* path);
void unload_texture(Texture* texture);
bool lock_texture(Texture* texture);
bool unlock_texture(Texture* texture);

void draw_texture(Texture* texture, SDL_Rect target);

SDL_Color convert_colour(Uint32 colour);

#endif
