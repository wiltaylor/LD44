#ifndef __CONSOLE_H_
#define __CONSOLE_H_
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool init_console(SDL_Rect dims);
void shutdown_console();
void toggle_console(bool active);
void update_console();
void append_text(char* text);
void set_commandline(char* text);
#endif
