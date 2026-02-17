/**
 * Entry point for ray tracing logic. Started as pthread from RayTracer.c
 * Separates SDL Window thread and code from ray tracing code.
 * The goal is to make this ultra-portable so it can be transferred to the DE1-SOC ARM
 * with display via VGA
 */
#include "Vec3.h"
#include "Ray.h"
#include "Colors.h"
#include "RayRenderer.h"
#include "../Viewport.h"
#include <stdio.h>

const fix15 image_height = int2fix(WINDOW_HEIGHT);
const fix15 image_width = int2fix(WINDOW_WIDTH);
const fix15 aspect_ratio = divfix(image_width, image_height);

const fix15 focal_length = int2fix(1.0);
const fix15 vp_height = float2fix(2.0);
const fix15 vp_width = multfix(vp_height, aspect_ratio);

Vec3 camera = {0,0,0};
Vec3 focal_vec = {0, 0, -focal_length};

Vec3 vp_ud2 = {multfix(half, vp_width), 0, 0};
Vec3 vp_du = {divfix(vp_width, image_width), 0, 0};

Vec3 vp_vd2 = {0, multfix(half, vp_height), 0};
Vec3 vp_dv = {0, - divfix(vp_height, image_height), 0};

Vec3 vp_upper_left;
Vec3 vp_pixel;

Vec3 sp_center = {0,0,int2fix(-5)};
Sphere sp = {&sp_center, int2fix(2), red};

Vec3 cam_step = {0, 0, float2fix(0.01)};

uint16_t linear_interpolate_color(color_t C1, color_t C2, fix15 t)
{
    fix15 j = one - t;
    color_t res;

    res.R = multfix(t, C1.R) + multfix(j, C2.R);
    res.G = multfix(t, C1.G) + multfix(j, C2.G);
    res.B = multfix(t, C1.B) + multfix(j, C2.B);
    res.A = C1.A & C2.A;

    return ENCODE(res);
}

uint16_t ray_color(int x, int y)
{
    color_t C1 = white;
    color_t C2 = skyblue;

    fix15 t = divfix(int2fix(y), int2fix(WINDOW_HEIGHT));

    return linear_interpolate_color(C1, C2, t);

}

void trace(uint16_t *frame_buffer, int x, int y, Vec3 *vp_pixel)
{
    Ray ray = {&camera, vp_pixel, 0};
    fix15 t = ray_sphere_intersect(&ray, &sp);
    if (t >= 0)
    {
        Vec3 hit = ray_at(&ray, t);
        Vec3 n = sphere_normal(&hit, &sp);

        uint16_t color = linear_interpolate_color(white, gray, n.y);

        frame_buffer[x + (y * WINDOW_WIDTH)] = color;
    }
    else
    {
        frame_buffer[x + (y * WINDOW_WIDTH)] = ENCODE(skyblue);
    }
}

void *entry(void *frame_buffer)
{
    uint16_t *fb = (uint16_t *) frame_buffer;
    int dir = 0;

    while(1)
    {
        addVec(&vp_upper_left, &camera, &focal_vec);
        subVec(&vp_upper_left, &vp_upper_left, &vp_ud2);
        addVec(&vp_upper_left, &vp_upper_left, &vp_vd2);
        
        Vec3 row_start = vp_upper_left;
    
        for(int y = 0; y < WINDOW_HEIGHT; y++)
        {
            vp_pixel = row_start;
            for (int x = 0; x < WINDOW_WIDTH; x++)
            {
                trace(fb, x, y, &vp_pixel);
                addVec(&vp_pixel, &vp_pixel, &vp_du);
            }
            addVec(&row_start, &row_start, &vp_dv);
        }

        if (camera.z > two)
        {
            dir = 0;
        }
        else if (camera.z < n_two)
        {
            dir = 1;
        }
        
        if (dir)
        {
            addVec(&camera, &camera, &cam_step);
        }
        else
        {
            subVec(&camera, &camera, &cam_step);
        }
        
    }
}