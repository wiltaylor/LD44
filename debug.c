#include "debug.h"
#include "fontrender.h"
#include "pixelrenderer.h"
#include "player.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

static char* player_location;
static Uint32 text_colour;
static TTF_Font* font;

void init_debug()
{
    player_location = malloc(sizeof(char) * 50);
    player_location[0] = 0;

    text_colour = 0;

    font = load_font("assets/neoletters.ttf", 18);
}

void update_debug()
{
    if(text_colour == 0)
        text_colour = set_colour(0x00, 0xFF, 0x00, 0x00);

    vec3* pos = player_pos();
    float* ang = player_angle();

    int len = sprintf(player_location, "X: %f, y: %f, z: %f, a: %f", pos->x, pos->y, pos->z, *ang);

    draw_text(0,0, player_location, text_colour, font);
}

void shutdown_debug()
{
    free(player_location);
    unload_font(font);
}
