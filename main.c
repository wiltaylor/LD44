#include <stdio.h>
#include "render.h"
#include "input.h"
#include "pixelrenderer.h"
#include "texture.h"
#include "fontrender.h"
#include "console.h"
#include "command.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Texture* test_tex = NULL;
SDL_Rect test_rec;
TTF_Font* test_font = NULL;

void mainloop()
{
        

    update_input();
    start_frame();
    start_pixelrenderer();
    
    Uint32 red = set_colour(0xFF, 0x00, 0x00, 0xFF);
    Uint32 green = set_colour(0x00, 0xFF, 0x00, 0xFF);

    for(int i = 250; i < 300; i++)
    {
        write_pixel(i, 10, red, 10);
        write_pixel(10, i, green, 20);
    }

    end_pixelrenderer();
    draw_texture(test_tex, test_rec);
    draw_text(400,400, "Hey testing", red, test_font);
    
    update_console();
    end_frame();
    //DO loop stuff
}

int main(int argc, char** arg) 
{
    //init stuff
    if(!init_render(800, 600, "Ludum Dare 44"))
        return -1;

    if(!init_input())
        return -1;

    test_tex = load_texture("assets/test.png");
    if(test_tex == NULL)
        return -1;

    test_font = load_font("assets/neoletters.ttf", 12);
    if(test_font == NULL)
        return -1;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 800;
    rect.h = 300;

    if(!init_console(rect))
        return -1; 
        
   //toggle_console(true); 

   execute_command("test \"foo bar\" bar bar two");


    test_rec.x = 50;
    test_rec.y = 50;
    test_rec.w = 100;
    test_rec.h = 100;
    
    //Loop logic
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
    #else
    while(!quiting()) {
        mainloop();
    }
    
    shutdown_input();
    shutdown_render();

    //Shutdown code for exe must be inside conditional or it will run when emscripten exists.
    #endif

    return 0;
}
