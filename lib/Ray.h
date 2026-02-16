#ifndef RAY_H
#define RAY_H

#include "Fix.h"
#include "Vec3.h"

typedef struct 
{
    Vec3 *origin;
    Vec3 *target;
    fix15 length;
} Ray;

Vec3 direction(Ray *ray)
{
    Vec3 *dir;

    subVec(dir, ray->target, ray->origin);
    normalize(dir);
}

#endif RAY_H