#include <stdio.h>
#include "render.h"
#include "input.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void mainloop()
{
    update_input();
    start_frame();

    end_frame();
    //DO loop stuff
}

int main(int argc, char** arg) 
{
    //init stuff
    if(!init_render(640, 480, "Ludum Dare 44"))
        return;

    if(!init_input())
        return;

    
    
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
