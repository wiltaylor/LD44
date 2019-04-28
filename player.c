#include "player.h"
#include "timer.h"

static vec3 position;
static vec3 velocity;
static float angle;
static float yaw;

vec3* player_pos()
{
    return &position;
}

vec3* player_vel()
{
    return &velocity;
}

float* player_angle()
{
    return &angle;
}

float* player_yaw()
{
    return &yaw;
}

void update_player()
{
    float deltatime = get_deltatime();

    position.x += velocity.x * deltatime;
    position.y += velocity.y * deltatime;

    velocity.x = 0;
    velocity.y = 0;
}
