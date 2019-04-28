#include "input.h"
#include <SDL2/SDL.h>
#include "console.h"
#include <malloc.h>
#include "command.h"
#include "gamemanager.h"
#include "player.h"

static bool consoleflag = false;

static SDL_Event e;
static char* text;

bool init_input()
{
    text = malloc(sizeof(char) * 100);
    text[0] = 0;
    set_commandline(text);

    //nothing yet
    return true;
}

void update_input()
{
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
        {
            quit_game();
        }
        if(e.type == SDL_TEXTINPUT)
        {
            if(!consoleflag)
                continue;

            if(strcmp(e.text.text, "`") == 0)
                continue;

            strcat(text, e.text.text);
            set_commandline(text);
        }
        
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_w:
                    if(!consoleflag)
                    {
                        float* ang = player_angle();
                        vec3* vel = player_vel();
                        vel->x += cosf(*ang) * 0.2f;
                        vel->y += sinf(*ang) * 0.2f; 
                    }
                    break;
                case SDLK_s:
                    if(!consoleflag)
                    {
                        float* ang = player_angle();
                        vec3* vel = player_vel();
                        vel->x -= cosf(*ang) * 0.2f;
                        vel->y -= sinf(*ang) * 0.2f;
                    }
                    break;
                case SDLK_a:
                    if(!consoleflag)
                    {
                        float* ang = player_angle();
                        *ang += 0.01;

                    }
                    break;
                case SDLK_d:
                    if(!consoleflag)
                    {
                        float* ang = player_angle();
                        *ang -= 0.01;
                    }
                    break;
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

