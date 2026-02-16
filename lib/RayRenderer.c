/**
 * Entry point for ray tracing logic. Started as pthread from RayTracer.c
 * Separates SDL Window thread and code from ray tracing code.
 * The goal is to make this ultra-portable so it can be transferred to the DE1-SOC ARM
 * with display via VGA
 */
#include "Vec3.h"
// #include "Ray.h"
#include "Colors.h"
#include "RayRenderer.h"
#include "../Viewport.h"
#include <stdio.h>

uint16_t ray_color(int x, int y)
{
    
}

void trace(uint16_t *frame_buffer)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            frame_buffer[x + (y * WINDOW_WIDTH)] = PURPLE;
        }
    }
}

void *entry(void *frame_buffer)
{
    uint16_t *fb = (uint16_t *) frame_buffer;

    const fix15 image_height = int2fix(WINDOW_HEIGHT);
    const fix15 image_width = int2fix(WINDOW_WIDTH);
    const fix15 aspect_ratio = divfix(image_height, image_width);

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
    subVec(&vp_upper_left, &camera, &focal_vec);
    subVec(&vp_upper_left, &vp_upper_left, &vp_ud2);
    subVec(&vp_upper_left, &vp_upper_left, &vp_vd2);

    for(;;)
    {
        trace(fb);
    }
}