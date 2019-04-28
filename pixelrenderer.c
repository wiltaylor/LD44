#include "pixelrenderer.h"
#include <stdio.h>
#include <malloc.h>
#include "gamemaths.h"

static SDL_Texture* render_texture = NULL;
static SDL_Renderer* renderer = NULL;
static Uint32* pixels = NULL;
static float* depth_buffer = NULL;

static int pitch = 0;

static int screen_width = 0;
static int screen_height = 0;
static float max_depth = 0;
static Uint32 pix_format = 0;
static SDL_PixelFormat* mapping_format = NULL;

bool init_pixelrenderer(SDL_Renderer* r, SDL_Window* window, int width, int height, float depth)
{

    renderer = r;
    screen_width = width;
    screen_height = height;
    max_depth = depth;

    pix_format = SDL_GetWindowPixelFormat(window);


    render_texture = SDL_CreateTexture(r, pix_format, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (render_texture == NULL)
    {
        printf("Failed to create pixel renderer texture!");
        return false;
    }

    depth_buffer = malloc(sizeof(float) * (width * height));

    return true;
}

void shutdown_pixelrenderer()
{
    if(render_texture != NULL)
        SDL_DestroyTexture(render_texture);

    renderer = NULL;
    render_texture = NULL;
}

void start_pixelrenderer()
{
    SDL_LockTexture(render_texture, NULL, (void**)&pixels, &pitch);
    mapping_format = SDL_AllocFormat(pix_format);

    Uint32 black = set_colour(0x00, 0x00, 0x00, 0xFF);

    for(int i = 0; i < screen_width * screen_height; i++)
    {
        pixels[i] = black;
        depth_buffer[i] = max_depth;
    }
}

void end_pixelrenderer()
{
    SDL_UnlockTexture(render_texture);
    pixels = NULL;

    SDL_RenderCopy(renderer, render_texture, NULL, NULL);
}

void force_write(int x, int y, unsigned int colour, float depth)
{
    if(x < 0 || y < 0 || x >= screen_width || y >= screen_height)
        return; 

    int index = x + y * screen_width;

    pixels[index] = colour;
    depth_buffer[index] = depth;
}

bool write_pixel(int x, int y, unsigned int colour, float depth)
{
    if(x < 0 || y < 0 || x >= screen_width || y >= screen_height)
        return;

   int index = x + y * screen_width;

   if(depth_buffer[index] > depth)
   {
       pixels[index] = colour;
       depth_buffer[index] = depth;
       return true;
   }

   return false;
}

Uint32 set_colour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_MapRGBA(mapping_format, r, g, b, a);
}

void get_colours(Uint32 colour, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    SDL_GetRGBA(colour, mapping_format, r, g, b, a);
}

void draw_line(int x0, int y0, int x1, int y1, Uint32 colour)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2 * dy - dx;
    int y = y0;

    for(int x = x0; x <= x1; x++)
    {
        force_write(x, y, colour, 0);
        if(D > 0)
        {
            y = y + 1;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
    /*
   vec2 diff;
   diff.x = x1 + x2;
   diff.y = y1 + y2;

   vec2 dir = vec2_norm(&diff);
   float dist = vec2_mag(&diff);

   vec2 point;
   point.x = x1;
   point.y = y1;

   while(dist > 0)
   {
        force_write((int)point.x, (int)point.y, colour, 0);
        dist -= 1;
        point.x += 1 * dir.x;
        point.y += 1 * dir.y;
   }  
   */
}
