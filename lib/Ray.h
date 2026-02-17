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
        return divfix(-b - fixSqrt(disc), multfix(two, a)); 
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