#ifndef __RENDER_H_
#define __RENDER_H_ 
#include <stdbool.h>

bool init_render(int width, int height, char* title);
void shutdown_render();

void start_frame();
void end_frame();
#endif
