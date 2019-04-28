#include "timer.h"

static Uint64 last = 0;
static float deltatime = 0;

void update_timer()
{
    Uint64 now = SDL_GetPerformanceCounter();
    deltatime = (float)((now - last)*1000 / (double)SDL_GetPerformanceFrequency());
    last = now;
}

float get_deltatime()
{
    return deltatime;
}
