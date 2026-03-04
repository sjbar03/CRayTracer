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

#define RAY_PER_PX 20
#define REFLECT_DPTH 20

const fix15 image_height = int2fix(WINDOW_HEIGHT);
const fix15 image_width = int2fix(WINDOW_WIDTH);
const fix15 aspect_ratio = divfix(image_width, image_height);

const fix15 focal_length = float2fix(1.5);
const fix15 vp_height = float2fix(2.0);
const fix15 vp_width = multfix(vp_height, aspect_ratio);

Vec3 camera = {0, 0, int2fix(5)};
Vec3 focal_vec = {0, 0, -focal_length};

Vec3 vp_ud2 = {multfix(half, vp_width), 0, 0};
Vec3 vp_du = {divfix(vp_width, image_width), 0, 0};

Vec3 vp_vd2 = {0, multfix(half, vp_height), 0};
Vec3 vp_dv = {0, - divfix(vp_height, image_height), 0};

Vec3 vp_upper_left;
Vec3 vp_pixel;

// basis vectors
Vec3 bx = {one, 0, 0};
Vec3 by = {0, one, 0};
Vec3 bz = {0, 0, one};

Vec3 sp1_center = {0,float2fix(0.1),int2fix(-5)};
const Sphere sp1 = {&sp1_center, float2fix(2.0), skyblue, 0};

Vec3 sp2_center = {0, int2fix(-100), int2fix(-10)};
const Sphere sp2 = {&sp2_center, float2fix(98.0), gray, 0};

Vec3 sp3_center = {int2fix(5), 0, int2fix(-5)};
const Sphere sp3 = {&sp3_center, float2fix(1), white, one};

Sphere sps[3] = {sp1, sp2, sp3};

Vec3 cam_step = {0, 0, float2fix(0.01)};


fix15 clamp(fix15 val, fix15 min, fix15 max)
{
    if (min <= val && max >= val) return val;
    if (min > val) return min;
    return max;

}

color_t ray_color(Ray *ray, int depth)
{
    if (depth <= 0) return black;

    for (int i = 0; i < 3; i++)
    {
        Sphere sp = sps[i];
        
        fix15 t = ray_sphere_intersect(ray, &sp);
        
        if (t >= float2fix(0.001)) {

            Vec3 hp = ray_at(ray, t);
            Vec3 n = sphere_normal(&hp, &sp);
            normalize(&n);

            Vec3 rv = random_on_hemisphere(&n); 
            addVec(&rv, &rv, &n);
            addVec(&rv, &rv, &hp); // make new target in this direction

            Ray rr = {&hp, &rv, 0};

            color_t rc = ray_color(&rr, depth - 1);

            return color_scale(rc, float2fix(0.6));
        }
    }
    
    Vec3 uv = direction(ray);
    normalize(&uv);

    fix15 a = multfix(dot(&by, &uv) + one, half);
    return linear_interpolate_color(white, skyblue, a);
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

                fb[x + (y * WINDOW_WIDTH)] = ENCODE(gamma_correct(c));
                addVec(&vp_pixel, &vp_pixel, &vp_du);
            }
            addVec(&row_start, &row_start, &vp_dv);
        } 

        
    } while(0);

    while(!gDone);

    return NULL;
}
