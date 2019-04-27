#include "texture.h"
#include <malloc.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_PixelFormat* screen_format = NULL;

bool init_texture(SDL_Window* win, SDL_Renderer* ren)
{
    window = win; 
    renderer = ren;

    Uint32 format = SDL_GetWindowPixelFormat(window);
    screen_format = SDL_AllocFormat(format);

    return true;
}

void shutdown_texture()
{
    window = NULL;
    renderer = NULL;
}

Texture* load_texture(char* path)
{
    Texture* result = malloc(sizeof(Texture));
    result->pixels = NULL;
    
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL) {
        printf("Failed to load surface %s\n", path);
        return NULL;
    }

    SDL_Surface* formatedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(window), 0);
    if(formatedSurface == NULL)
    {
        printf("Failed to load surface %s was unable to convert format!\n", path);
        return NULL;
    }

    result->width = formatedSurface->h;
    result->height = formatedSurface->w;
    result->rect.x = 0;
    result->rect.y = 0;
    result->rect.w = result->width;
    result->rect.h = result->height;

    result->texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, formatedSurface->w, formatedSurface->h);
    if(result->texture == NULL)
    {
        printf("Failed to create texture for %s\n", path);
        return NULL;
    }

    if(!lock_texture(result))
    {
        printf("Failed to lock texture during init %s\n", path);
        return NULL;
    }

    memcpy(result->pixels, formatedSurface->pixels, formatedSurface->pitch * formatedSurface->h);
    
    if(!unlock_texture(result)) 
    {
        printf("Failed to unlock texture during init\n");
        return NULL;
    }

    SDL_FreeSurface(loadedSurface);
    SDL_FreeSurface(formatedSurface);

    return result;
}

void unload_texture(Texture* texture)
{
    if(texture == NULL)
        return;
    if(texture->texture != NULL)
        SDL_DestroyTexture(texture->texture);

    texture->texture = NULL;
    texture->pixels = NULL;
}

bool lock_texture(Texture* texture)
{
    if(texture == NULL)
        return false;

    if(texture->pixels != NULL)
        return false;

    SDL_LockTexture(texture->texture, NULL, (void**)&texture->pixels, &texture->pitch);
    
    Uint32 format = SDL_GetWindowPixelFormat(window);
    texture->format = SDL_AllocFormat(format);

    return true;
}

bool unlock_texture(Texture* texture)
{
    if(texture == NULL)
        return false;

    if(texture->pixels == NULL)
        return false;

    SDL_UnlockTexture(texture->texture);
    texture->pixels = NULL;

    return true;
}

void draw_texture(Texture* texture, SDL_Rect target)
{
    SDL_RenderCopy(renderer, texture->texture, &texture->rect, &target);
}

SDL_Color convert_colour(Uint32 colour)
{
    SDL_Color result;

    SDL_GetRGBA(colour, screen_format, &result.r, &result.g, &result.b, &result.a);
  
    return result;
}
