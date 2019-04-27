#include "input.h"
#include <SDL2/SDL.h>

bool quitflag = false;

SDL_Event e;

bool init_input()
{
    //nothing yet
    return true;
}

void update_input()
{
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
        {
            quitflag = true;
        }
    }
}

void shutdown_input()
{
    //Do shutdown code 
}

bool quiting()
{
    return quitflag;
}
