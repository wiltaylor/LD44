#include "render.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "pixelrenderer.h"
#include "texture.h"
#include "fontrender.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static int screen_width = 0;
static int screen_height = 0;

bool init_render(int width, int height, char* title)
{
    screen_width = width;
    screen_height = screen_height;

    if(SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        printf("Failed to init SDL video!\n");
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        printf("Unable to create SDL Window\n");
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Failed to create renderer!\n");
        return false;
    }


    if(!IMG_Init(IMG_INIT_PNG))
    {
        printf("Unable to laod image loading library!\n");
        return false;
    }

    if(!init_pixelrenderer(renderer, window, width, height, 100.0f))
    {
        printf("Failed to load pixel renderer!\n");
        return false;
    }

    if(!init_texture(window, renderer))
    {
        printf("Failed to load texture handler!\n");
        return false;
    }

    if(!init_font(renderer))
    {
        printf("Failed to load font renderer!\n");
        return false;
    }

    return true;
}

void shutdown_render()
{
    shutdown_pixelrenderer();
    shutdown_texture();
    shutdown_font();

    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    
    if(window != NULL)
        SDL_DestroyWindow(window);

    renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void start_frame()
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

void end_frame()
{
    SDL_RenderPresent(renderer);
}
