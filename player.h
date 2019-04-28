#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "gamemaths.h"

vec3* player_pos();
vec3* player_vel();
float* player_angle();
float* player_yaw();

void update_player();

#endif
