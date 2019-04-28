#ifndef __GAMEMATHS_H_
#define __GAMEMATHS_H_

typedef struct  
{
    float x;
    float y;
}vec2;

typedef struct 
{
    float x;
    float y;
    float z;
}vec3;

float vec2_mag(vec2* original);
vec2 vec2_norm(vec2* original);
float vec2_cross(vec2* a, vec2* b);
vec2 vec2_intersect(vec2* line1_start, vec2* line1_end, vec2* line2_start, vec2* line2_end);

#define min(a,b)                (((a) < (b)) ? (a) : (b))
#define max(a,b)                (((a) > (b)) ? (a) : (b))
#define clamp(a, mi, ma)        min(max(a, mi), ma)
#endif
