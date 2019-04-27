#include "console.h"
#include "texture.h"
#include "fontrender.h"
#include <stdio.h>
#include <string.h>
#include "pixelrenderer.h"

static Texture* background_texture = NULL;
static bool console_active = false;

static Uint32 console_colour = 0;
static TTF_Font* console_font = NULL;
static SDL_Rect console_dimensions;
static char** text_buffer;
static char command_line[101];
static int text_size = 20;
static int text_rows = 0;
static int text_cols = 0;

void drawline(int row, char* text)
{
    draw_text(0, console_dimensions.h - (text_size + 2) * row, text, console_colour, console_font);
}

void set_commandline(char* text)
{
    strcpy(command_line, "> ");
    strcat(command_line, text);
}

void append_text(char* text)
{
    printf("CONSOLE: %s\n", text); 

    int len = strlen(text);

    //Truncate string so it doesn't overflow.
    if(len > text_cols)
    {
        len = text_cols;
    }

    if(text_buffer[text_rows - 1] != NULL)
        free(text_buffer[text_rows - 1]);

    for(int i = text_rows - 2; i > 0; i--)
    {
        text_buffer[i] = text_buffer[i -1];
    }
    
    text_buffer[0] = malloc(len + 1);
    strncpy(text_buffer[0], text, len);
    text_buffer[0][len] = 0;
    
    
}

bool init_console(SDL_Rect dims) 
{


    console_dimensions = dims;

    text_rows = dims.h / (text_size + 2);
    text_cols = dims.w / (text_size);

    text_buffer = malloc(sizeof(char*) * text_rows);
    
    console_font = load_font("assets/neoletters.ttf", text_size);
    background_texture = load_texture("assets/console.png");

    if(background_texture == NULL)
        return false;

    for(int i = 0; i < text_rows; i++)
        text_buffer[i] = NULL;

    
    append_text("Starting game...");
    set_commandline("");


    return true;
}

void shutdown_console()
{
    if(background_texture != NULL)
        unload_texture(background_texture);
    if(console_font != NULL)
        unload_font(console_font);
    
    background_texture = NULL;
    console_font = NULL;
}

void toggle_console(bool active)
{
    console_active = active;
}

void update_console()
{
    if(console_colour == 0)
        console_colour = set_colour(0xFF, 0xFF, 0xFF, 0xFF);

    if(!console_active)
        return;

    draw_texture(background_texture, console_dimensions);

    //draw_text(0, console_dimensions.h - 12, "> Testing Testing", console_colour, console_font);
    
    for(int i = 2; i < text_rows + 2; i++)
        if(text_buffer[i-2] != NULL)
            drawline(i, text_buffer[i-2]);
        
    drawline(1, command_line);

}


