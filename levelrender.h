#ifndef __LEVELRENDER_H_
#define __LEVELRENDER_H_
#include "gamemaths.h"

void init_level();
void update_level();

typedef struct {
    float floor_height;
    float ceil_height;
    vec2* verts;
    unsigned int vert_count;
}sector;

#endif
