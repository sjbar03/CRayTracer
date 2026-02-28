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
    Vec3 oc; subVec(&oc, sphere->center, ray->origin);

    Vec3 dir = direction(ray);
    normalize(&dir);

    fix15 h = dot(&dir, &oc);
    fix15 c = dot(&oc, &oc) - square(sphere->radius);
    fix15 disc = square(h) - c;

    if (disc < 0) return n_one;
    
    return h - fixSqrt(disc);
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