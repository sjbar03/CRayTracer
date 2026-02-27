#include "Vec3.h"

void addVec(Vec3 *r, Vec3 *a, Vec3 *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
    r->z = a->z + b->z;
}

void subVec(Vec3 *r, Vec3 *a, Vec3 *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
}

void accVec(Vec3 *a, Vec3 *b)
{
    a->x = a->x + b->x;
    a->y = a->y + b->y;
    a->z = a->z + b->z;
}

fix15 dot(Vec3 *a, Vec3 *b)
{
    return multfix(a->x, b->x) + multfix(a->y, b->y) + multfix(a->z, b->z);
}

fix15 L2(Vec3 *a)
{
    return fixSqrt(square(a->x) + square(a->y) + square(a->z));
}

void cross(Vec3 *r, Vec3 *a, Vec3 *b)
{
    r->x = multfix(a->y, b->z) - multfix(a->z, b->y);
    r->y = multfix(a->z, b->x) - multfix(a->x, b->z);
    r->z = multfix(a->x, b->y) - multfix(a->y, b->x);
}

void normalize(Vec3 *a)
{
    fix15 invNorm = rfixSqrt(square(a->x) + square(a->y) + square(a->z));
    a->x = multfix(a->x, invNorm);
    a->y = multfix(a->y, invNorm);
    a->z = multfix(a->z, invNorm);
}

void normalizeTo(Vec3 *a, Vec3 *b)
{
    fix15 invNorm = rfixSqrt(square(a->x) + square(a->y) + square(a->z));
    b->x = multfix(a->x, invNorm);
    b->y = multfix(a->y, invNorm);
    b->z = multfix(a->z, invNorm);
}

void scale(Vec3 *a, fix15 s)
{
    a->x = multfix(a->x, s);
    a->y = multfix(a->y, s);
    a->z = multfix(a->z, s);
}