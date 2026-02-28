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

#define RAY_PER_PX 100 
#define REFLECT_DPTH 5

const fix15 image_height = int2fix(WINDOW_HEIGHT);
const fix15 image_width = int2fix(WINDOW_WIDTH);
const fix15 aspect_ratio = divfix(image_width, image_height);

const fix15 focal_length = int2fix(1.0);
const fix15 vp_height = float2fix(2.0);
const fix15 vp_width = multfix(vp_height, aspect_ratio);

Vec3 camera = {int2fix(3),0,0};
Vec3 focal_vec = {0, 0, -focal_length};

Vec3 vp_ud2 = {multfix(half, vp_width), 0, 0};
Vec3 vp_du = {divfix(vp_width, image_width), 0, 0};

Vec3 vp_vd2 = {0, multfix(half, vp_height), 0};
Vec3 vp_dv = {0, - divfix(vp_height, image_height), 0};

Vec3 vp_upper_left;
Vec3 vp_pixel;

Vec3 sp1_center = {0,0,int2fix(-5)};
const Sphere sp1 = {&sp1_center, int2fix(2), skyblue};

Vec3 sp2_center = {0, int2fix(-100), int2fix(-10)};
const Sphere sp2 = {&sp2_center, int2fix(97), gray};

Sphere sps[2] = {sp1, sp2};

Vec3 cam_step = {0, 0, float2fix(0.01)};

color_t linear_interpolate_color(color_t C1, color_t C2, fix15 t)
{
    fix15 j = one - t;
    color_t res;

    res.R = multfix(t, C1.R) + multfix(j, C2.R);
    res.G = multfix(t, C1.G) + multfix(j, C2.G);
    res.B = multfix(t, C1.B) + multfix(j, C2.B);
    res.A = C1.A & C2.A;

    return res;
}

color_t color_scale(color_t in, fix15 scale)
{
    return (color_t) {
        true,
        multfix(in.R, scale),
        multfix(in.G, scale),
        multfix(in.B, scale)
    };
}

color_t add_color(color_t in1, color_t in2)
{
    return (color_t) {
        true,
        in1.R + in2.R,
        in1.G + in2.G,
        in1.B + in2.B
    };
}

// color_t ray_color(int x, int y)
// {
//     color_t C1 = white;
//     color_t C2 = skyblue;

//     fix15 t = divfix(int2fix(y), int2fix(WINDOW_HEIGHT));

//     return linear_interpolate_color(C1, C2, t);

// }

color_t ray_color(Ray *ray, int depth)
{
    if (depth <= 0) return black;

    for (int i = 0; i < 2; i++)
    {
        Sphere sp = sps[i];
        
        fix15 t = ray_sphere_intersect(ray, &sp);
        
        if (t>=0) {
            Vec3 hp = ray_at(ray, t);
            Vec3 n = sphere_normal(&hp, &sp);
            normalize(&n);

            Vec3 rv = random_on_hemisphere(&n); 
            addVec(&rv, &rv, &hp); // make new target in this direction

            Ray rr = {&hp, &rv, 0};

            color_t rc = ray_color(&rr, depth - 1);

            return color_scale(rc, half);
        }
    }
    
    Vec3 uv = direction(ray);
    normalize(&uv);

    fix15 a = multfix(half, uv.y + one);
    return linear_interpolate_color(babyblue, white, a);
}

color_t trace(Ray *ray)
{
    return ray_color(ray, REFLECT_DPTH);
}

void *entry(void *frame_buffer)
{
    raw_color_t *fb = (raw_color_t *) frame_buffer;
    // int dir = 0;

    fix15 sample_scale = divfix(one, int2fix(RAY_PER_PX));

    do
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
                color_t c = black;
                for (int sample = 0; sample < RAY_PER_PX; sample++)
                {
                    Ray r = {
                        &camera,
                        &vp_pixel,
                        0
                    };

                    c = add_color(c, trace(&r));   
                }

                c = color_scale(c, sample_scale);

                fb[x + (y * WINDOW_WIDTH)] = ENCODE(c);
                addVec(&vp_pixel, &vp_pixel, &vp_du);
            }
            addVec(&row_start, &row_start, &vp_dv);
        } 

        // if (camera.z > two)
        // {
        //     dir = 0;
        // }
        // else if (camera.z < n_two)
        // {
        //     dir = 1;
        // }
        
        // if (dir)
        // {
        //     addVec(&camera, &camera, &cam_step);
        // }
        // else
        // {
        //     subVec(&camera, &camera, &cam_step);
        // }
        
    } while(0);

    while(!gDone);

    return NULL;
}
