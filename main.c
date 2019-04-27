#include <stdio.h>
#include "render.h"
#include "input.h"
#include "pixelrenderer.h"
#include "texture.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Texture* test_tex = NULL;
SDL_Rect test_rec;

void mainloop()
{
    update_input();
    start_frame();
    start_pixelrenderer();
    
    Uint32 red = set_colour(0xFF, 0x00, 0x00, 0xFF);
    Uint32 green = set_colour(0x00, 0xFF, 0x00, 0xFF);
    for(int i = 10; i < 100; i++)
    {
        write_pixel(i, 10, red, 10);
        write_pixel(10, i, green, 20);
    }

    end_pixelrenderer();
    draw_texture(test_tex, test_rec);
    end_frame();
    //DO loop stuff
}

int main(int argc, char** arg) 
{
    //init stuff
    if(!init_render(640, 480, "Ludum Dare 44"))
        return -1;

    if(!init_input())
        return -1;

    test_tex = load_texture("assets/test.png");
    if(test_tex == NULL)
        return -1;

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
