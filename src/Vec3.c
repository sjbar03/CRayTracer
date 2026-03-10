#include "Vec3.h"
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

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
    // printVec((*a));
    assert(abs(a->x) < int2fix(128) && abs(a->y) < int2fix(128) && abs(a->z) < int2fix(128)); // assert overflow won't happen

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

Vec3 randomvec()
{
    Vec3 r;
    
    uint8_t signs = rand();

    do {
        r = (Vec3) {
                (fix15) (((int32_t) rand()) >> 16),
                (fix15) (((int32_t) rand()) >> 16),
                (fix15) (((int32_t) rand()) >> 16),
            };
    } while (r.x == 0 && r.y == 0 && r.z == 0);

    if (signs & 0x1) r.x = multfix(r.x, n_one);
    if (signs & 0x2) r.y = multfix(r.y, n_one);
    if (signs & 0x4) r.z = multfix(r.z, n_one);

    return r;
}

Vec3 random_unit_vector()
{
    while(1)
    {
        Vec3 p = randomvec();
        fix15 lensq = dot(&p, &p);

        if ((float2fix(0.001) < lensq) && (lensq <= one)) {
            normalize(&p);
            return p;
        }
    }
}

Vec3 random_on_hemisphere(Vec3 *normal)
{
    Vec3 p = random_unit_vector();

    if (dot(&p, normal) > 0) return p; // on hemisphere

    scale(&p, n_one);
    return p;
}

void findOrthogonal(Vec3 *r, Vec3 *n)
{
    assert(!(n->x == 0 && n->y == 0 && n->z == 0));

    fix15 nx = abs(n->x);
    fix15 ny = abs(n->y);
    fix15 nz = abs(n->z);

    if (nx <= ny && nx <= nz) { *r = (Vec3) {0, n->z, -n->y}; }
    else if (ny <= nz)        { *r = (Vec3) {-n->z, 0, n->x}; }
    else                      { *r = (Vec3) {n->y, -n->x, 0}; }

    // assert(dot(r, n) == 0); // make sure dot product is zero
    // assert(!(r->x == 0 && r->y == 0 && r->z == 0));
}

void randomSurf(Vec3 *r, Vec3 *n)
{
    assert(!(n->x == 0 && n->y == 0 && n->z == 0));

    Vec3 o1; findOrthogonal(&o1, n);
    Vec3 o2; cross(&o2, &o1, n);

    // Now we have orthogonal basis for the plane normal to n

    fix15 a = randomFix(int2fix(0.001) , one); // to prevent 0
    fix15 b = randomFix(n_one, one);
    fix15 c = randomFix(n_one, one);

    // linearally combine n, o1, and o2 with positive a coefficient
    // find random vector pointing in general direction of n (away from surface normal)
    *r = (Vec3) {
        multfix(a, n->x) + multfix(b, o1.x) + multfix(c, o2.x),
        multfix(a, n->y) + multfix(b, o1.y) + multfix(c, o2.y),
        multfix(a, n->z) + multfix(b, o1.z) + multfix(c, o2.z),
    };

}