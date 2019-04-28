#include "gamemaths.h"
#include <math.h>

float vec2_mag(vec2* original)
{
    return (float)sqrt(original->x * original->x + original->y * original->y);
}

vec2 vec2_norm(vec2* original)
{
    float mag = vec2_mag(original);

    vec2 result;
    result.x = original->x / mag;
    result.y = original->y / mag;

    return result;
}

float vec2_cross(vec2* a, vec2* b)
{
    return a->x * b->y - b->x * a->x;
}

vec2 vec2_intersect(vec2* line1_start, vec2* line1_end, vec2* line2_start, vec2* line2_end)
{
    vec2 result;

    float a = vec2_cross(line1_start, line1_end);
    float b = line1_start->x - line1_end->x;
    float c = vec2_cross(line2_start, line2_end);
    float d = line2_start->x - line2_end->y;
    vec2 part_a = (vec2){a, b};
    vec2 part_b = (vec2){c, d};
    float part_ab1 = vec2_cross(&part_a, &part_b);

    part_a.x = line1_start->x - line1_end->x;
    part_a.y = line1_start->y - line1_end->y;
    part_b.x = line2_start->x - line2_end->x;
    part_b.y = line2_start->y - line2_end->y;
    float part_ab2 = vec2_cross(&part_a, &part_b);

    result.x = part_ab1 / part_ab2;

    a = vec2_cross(line1_start, line1_end);
    b = line1_start->y - line1_end->y;
    c = vec2_cross(line2_start, line2_end);
    d = line2_start->y - line2_end->y;
    part_a = (vec2){a, b};
    part_b = (vec2){c, d};
    part_ab1 = vec2_cross(&part_a, &part_b);

    part_a.x = line1_start->x - line1_end->x;
    part_a.y = line1_start->y - line1_end->y;
    part_b.x = line2_start->x - line2_end->x;
    part_b.y = line2_start->y - line2_end->y;
    part_ab2 = vec2_cross(&part_a, &part_b);

    result.y = part_ab1 / part_ab2;

    return result;

}

