#include "input.h"
#include <SDL2/SDL.h>
#include "console.h"
#include <malloc.h>
#include "command.h"

static bool quitflag = false;
static bool consoleflag = false;

static SDL_Event e;
static char* text;

bool init_input()
{
    text = malloc(sizeof(char) * 100);
    text[0] = 0;

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
        if(e.type == SDL_TEXTINPUT)
        {
            strcat(text, e.text.text);
            set_commandline(text);
        }
        
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_RETURN:
                    if(consoleflag)
                    {
                        
                        execute_command(text);
                        text[0] = 0;
                        set_commandline(text);
                    }
                    break;
                case SDLK_BACKSPACE:
                    if(consoleflag)
                    {
                        int len = strlen(text);

                        if(len != 0)
                        {
                            text[len - 1] = 0;
                        }

                        set_commandline(text);
                    }
                    break;
                case SDLK_BACKQUOTE:
                    consoleflag = !consoleflag;
                    toggle_console(consoleflag);

                    if(consoleflag)
                    {
                        set_commandline(text);
                        SDL_StartTextInput();
                    }else{
                        SDL_StopTextInput();
                    }


                    break;
            }
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
