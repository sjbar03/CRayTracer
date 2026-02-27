#ifndef RAY_H
#define RAY_H

#include "Fix.h"
#include "Vec3.h"
#include "Colors.h"

typedef struct 
{
    Vec3 *origin;
    Vec3 *target;
    fix15 length;
} Ray;

typedef struct
{
    Vec3 *center;
    fix15 radius;
    color_t color;
} Sphere;

Vec3 direction(Ray *ray)
{
    Vec3 dir;
    subVec(&dir, ray->target, ray->origin);
    return dir;
}

fix15 ray_sphere_intersect(Ray *ray, Sphere *sphere)
{
    Vec3 ray_dir = direction(ray);

    Vec3 oc;
    subVec(&oc, sphere->center, ray->origin);

    fix15 a = dot(&ray_dir, &ray_dir);
    fix15 b_intermediate = dot(&ray_dir, &oc);
    fix15 b = multfix(n_two, b_intermediate);
    fix15 c = dot(&oc, &oc) - square(sphere->radius);
    fix15 ac = multfix(a, c);

    fix15 disc = square(b) - multfix(ac, four);

    if (disc < 0 || b_intermediate < 0)
    {
        return int2fix(-1);
    }
    else
    {
        fix15 two_a = multfix(two, a);
        fix15 sqrt_disc = fixSqrt(disc);
        fix15 t1 = divfix(-b - sqrt_disc, two_a);
        fix15 t2 = divfix(-b + sqrt_disc, two_a);

        if (t1 > 0) return t1;  // Closest hit in front of camera
        if (t2 > 0) return t2;  // Ray origin inside sphere
        return int2fix(-1);     // Both behind camera
    }
}

Vec3 sphere_normal(Vec3 *x, Sphere *sp)
{
    Vec3 res;
    subVec(&res, x, sp->center);
    normalize(&res);
    
    return res;
}

Vec3 ray_at(Ray *ray, fix15 t)
{
    Vec3 dir = direction(ray);
    normalize(&dir);
    scale(&dir, t);

    Vec3 res;
    addVec(&res, ray->origin, &dir);
    return res;
}

#endif