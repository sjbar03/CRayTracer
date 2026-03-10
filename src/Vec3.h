#ifndef VEC3_H
#define VEC3_H

#include "Fix.h"

typedef struct {
    fix15 x;
    fix15 y;
    fix15 z;
} Vec3;

#define printVec(v) printf("X: %f, Y: %f, Z: %f\n", fix2float(v.x), fix2float(v.y), fix2float(v.z))

/*
add vectors a and b and store them in r
r = a + b
*/
void addVec(Vec3 *r, Vec3 *a, Vec3 *b);

/*
subtract vectors a and b and store them in r
r = a - b
*/
void subVec(Vec3 *r, Vec3 *a, Vec3 *b);

/*
add value of b to a and store in a
a += b
*/
void accVec(Vec3 *a, Vec3 *b);

/*
dot product of vectors a and b
return a * b
*/
fix15 dot(Vec3 *a, Vec3 *b);

/*
cross product of a and store in r
r = a x b
*/
void cross(Vec3 *r, Vec3 *a, Vec3 *b);

/*
L2 norm of a
return sqrt(x^2 + y^2 + z^2)
*/
fix15 L2(Vec3 *a);

/*
Normalize vector a
Modifies a
*/
void normalize(Vec3 *a);

/*
Normalize vector a, store in r
*/
void normalizeTo(Vec3 *r, Vec3 *a);

/*
Scale vector a by scalar s, store in a
*/
void scale(Vec3 *a, fix15 s);

/*
Return a random vector with values in the range -1 - 1
*/
Vec3 randomvec();

Vec3 random_unit_vector();

Vec3 random_on_hemisphere(Vec3 *normal);

/*
Find any vector orthogonal to non-zero n
*/
void findOrthogonal(Vec3 *r, Vec3 *n);

/*
Generate random vector pointing away from surface (given normal)
*/
void randomSurf(Vec3 *r, Vec3 *n);

#endif