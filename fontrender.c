#include "fontrender.h"
#include <stdio.h>

static SDL_Renderer* renderer;

bool init_font(SDL_Renderer* rend)
{
    TTF_Init();
    renderer = rend;
    return true;
}

void shutdown_font()
{
    TTF_Quit();
    renderer = NULL;
}

TTF_Font* load_font(char* path, int size) 
{
    TTF_Font* result = TTF_OpenFont(path, size);
    if(result == NULL)
    {
        printf("Failed to load font!\n");
        return NULL;
    }

    return result;
}

void unload_font(TTF_Font* font)
{
    TTF_CloseFont(font);
}

Texture* render_text(char* text, Uint32 colour, TTF_Font* font)
{
    SDL_Color convertedcolour = convert_colour(colour);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, convertedcolour);
    if(textSurface == NULL)
    {
        printf("Unable to create surface from text\n");
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(texture == NULL)
    {
        printf("Unable to convert text surface to texture!\n");
        return NULL;
    }

    Texture* result = malloc(sizeof(Texture));
    result->pixels = NULL;
    result->texture = texture;
    result->width = textSurface->w;
    result->height = textSurface->h;
    result->rect.x = 0;
    result->rect.y =0;
    result->rect.w = textSurface->w;
    result->rect.h = textSurface->h;   
    
    SDL_FreeSurface(textSurface);

    return result;
}

void draw_text(int x, int y, char* text, Uint32 colour, TTF_Font* font)
{
    Texture* texture = render_text(text, colour, font);
    if(texture == NULL)
    {
        printf("Failed to write to screen. Text: %s\n", text);
        return;
    }

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = texture->width;
    dest.h = texture->height;

    SDL_RenderCopy(renderer, texture->texture, &texture->rect, &dest);

    unload_texture(texture);
}
