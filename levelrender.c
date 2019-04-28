#include "levelrender.h"
#include "pixelrenderer.h"
#include <math.h>
#include "player.h"
#include <stdio.h>

static int screen_width = 800;
static int screen_height = 600;
static int eye_height = 6;
static int head_margin = 1;
static int knee_margin = 2;
static float hfov = 0;
static float vfov = 0;;

static sector test_sec;

void init_level()
{
    hfov = 0.73f * screen_height;
    vfov = 0.2f * screen_height;

    vec3* p = player_pos();
    p->x = 0;
    p->y = 0;
    p->z = 0;

    vec3* v = player_vel();
    v->x = 0;
    v->y = 0;
    v->z = 0;

    float* ang = player_angle();
    *ang = 1.5;

    float* yaw = player_yaw();
    *yaw = 0;

    test_sec.ceil_height = 10;
    test_sec.floor_height = 0;
    test_sec.vert_count = 4;
    test_sec.verts = malloc(sizeof(vec2) * 4);

    test_sec.verts[0].x = -10;
    test_sec.verts[0].y = 10;
    test_sec.verts[1].x = 10;
    test_sec.verts[1].y = 10;
    test_sec.verts[2].x = 10;
    test_sec.verts[2].y = -10;
    test_sec.verts[3].x = -10;
    test_sec.verts[3].y = -10;
    

}

void draw_vline(int x, int y1, int y2, Uint32 colour)
{
   y1 = clamp(y1, 0, screen_height - 1);
   y2 = clamp(y2, 0, screen_height - 1);

   if(y2 == y1)
       force_write(x, y1, colour, 0);
   else if(y2 > y1)
   {
       force_write(x, y1, colour, 0);
       
       for(int y = y1 + 1; y < y2; y++)
           force_write(x,y, colour, 0);

       force_write(x, y2, colour, 0);
   }

}

void draw_sector(sector* sec)
{

    Uint32 wall_colour = set_colour(0xFF,0,0,0xFF);
    Uint32 floor_colour = set_colour(0x00, 0xFF, 0, 0xFF);
    Uint32 ceil_colour = set_colour(0x00, 0x00, 0xFF, 0xFF);



    vec3* player = player_pos();
    float* angle = player_angle();
    float* yaw = player_yaw();

    float pcos = cosf(*angle);
    float psin = sinf(*angle);

    int* ytop;
    int* ybottom;

    ytop = malloc(sizeof(int) * screen_width);
    ybottom = malloc(sizeof(int) * screen_width);

    for(int i = 0; i < screen_width; i++)
    {
        ytop[i] = 0;
        ybottom[i] = screen_height - 1;
    }
 
    for(int v = 0; v < sec->vert_count - 1; v++)
    {
        vec2 v1;
        vec2 v2;

        //Get vectors of wall and move it based on player location.
        v1.x = sec->verts[v].x - player->x;
        v1.y = sec->verts[v].y - player->y;

        v2.x = sec->verts[v + 1].x - player->x;
        v2.y = sec->verts[v + 1].y - player->y;

        //Rotate based on player rotation
        vec2 t1, t2;
        t1.x = v1.x * psin - v1.y * pcos;
        t1.y = v1.x * pcos + v1.y * psin;
        
        t2.x = v2.x * psin - v2.y * pcos;
        t2.y = v2.x * pcos - v2.y * psin;

        //Wall is totally behind player.
        if(t1.y <= 0 && t2.y <= 0)
            continue;

        //Checking if wall is partially behind player and cliping it.
        if(t1.y <=0 || t2.y <= 0)
        {
            //float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
            vec2 near = {1e-5f, 1e-4f};
            vec2 far = {20.0f, 5};
            vec2 negnear = {-1e-5f, -1e-4f};
            vec2 negfar = {-20.0f, -5};
            
            vec2 i1 = vec2_intersect(&t1, &t2, &negnear, &negfar);
            vec2 i2 = vec2_intersect(&t1, &t2, &near, &far);

            if(t1.y < near.y)
            {
                if(i1.y > 0)
                {
                    t1.x = i1.x;
                    t1.y = i1.y;
                }
                else
                {
                    t1.x = i2.x;
                    t1.y = i2.y;
                }
            }

            if(t2.y < near.y)
            {
                if(i1.y > 0)
                {
                    t2.x = i1.x;
                    t2.y = i1.y;
                }
                else
                {
                    t2.x = i2.x;
                    t2.y = i2.y;
                }
            }
        }

        //Perspective calculations
        vec2 scale1, scale2;
        scale1.x = hfov / t1.y;
        scale1.y = vfov / t1.y;

        scale2.x = hfov / t2.y;
        scale2.y = vfov / t2.y;
        
        float x1, x2;
        x1 = screen_width / 2 - (int)(t1.x * scale1.x);
        x2 = screen_width / 2 - (int)(t2.x * scale2.x);

        if(x1 >= x2)
            continue;

        if(x1 < 0 && x2 < 0)
            continue;

        //Getting floor and ceiling heights relative to player
        float ceil = sec->ceil_height - player->z;
        float floor = sec->floor_height - player->z;

        //do wall or portal check here
        

        int ya1,ya2, yb1, yb2;
        ya1 = screen_height / 2 - (int)((ceil + t1.y * *yaw) * scale1.y);
        yb1 = screen_height / 2 - (int)((floor + t1.y * *yaw) * scale1.y);
        ya2 = screen_height / 2 - (int)((ceil + t2.y * *yaw) * scale2.y);
        yb2 = screen_height / 2 - (int)((floor + t2.y * *yaw) * scale2.y);
        
        int beginx = max(x1, 0);
        int endx = min(x2, screen_width - 1);

        for(int x = beginx; x <= endx; x++)
        {
            //Getting y coords for ceiling and floor of this column
            int ya = (x - x1) * (ya2 - ya1) / (x2 - x1) + ya1;
            int yb = (x - x1) * (yb2 - yb1) / (x2 - x1) + yb1;
            int cya = clamp(ya, ytop[x], ybottom[x]);
            int cyb = clamp(yb, ytop[x], ybottom[x]);

            //Draw roof
            draw_vline(x, ytop[x], cya - 1, ceil_colour);

            //Draw floor
            draw_vline(x, cyb + 1, ybottom[x], floor_colour);

            //If solid wall
            draw_vline(x, cya, cyb, wall_colour);
        }
    }
}

void update_level()
{
    vec3* player = player_pos();
    float* angle = player_angle();
    
    Uint32 red = set_colour(0xFF,0,0,0xFF);
    Uint32 green = set_colour(0x00, 0xFF, 0, 0xFF);
    Uint32 blue = set_colour(0x00, 0x00, 0xFF, 0xFF);

    draw_sector(&test_sec);

    //draw_line(wall1.x, wall1.y, wall2.x, wall2.y, red);
    //draw_line(player.x, player.y, cos(angle) + player.x, sin(angle) + player.y, green);
    
    //draw_line(10, 10, 20, 10, red); 

/*    
    //Trasforming vertexes
    vec3 tw1;
    vec3 tw2;
    tw1.x = wall1.x - player->x;
    tw1.y = wall1.y - player->y;
    tw2.x = wall2.x - player->x;
    tw2.y = wall2.y - player->y;

    //Rotate around view
    tw1.z = tw1.x * cos(*angle) + tw1.y * sin(*angle);
    tw2.z = tw2.x * cos(*angle) + tw2.y * sin(*angle);
    tw1.x = tw1.x * sin(*angle) - tw1.y * cos(*angle);
    tw2.x = tw2.x * sin(*angle) - tw2.y * cos(*angle);

        //draw_line(0, 0, 800, 600, red);
   //
   
    int x1 = -tw1.x * 16 / tw1.z;
    int y1a = -50 / tw1.z;
    int y1b = 50 / tw1.z;

    int x2 = -tw2.x * 16 / tw2.z;
    int y2a = -50 / tw2.z;
    int y2b = 50 / tw2.z;

    draw_line(50 + x1, 50 + y1a, 50 + x2, 50 + y2a, red);
    draw_line(50 + x1, 50 + y1b, 50 + x2, 50 + y2b, red);
    draw_line(50 + x1, 50 + y1a, 50 + x1, 50 + y1b, red);
    draw_line(50 + x2, 50 + y2a, 50 + x2, 50 + y2b, red);
*/
}
