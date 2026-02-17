#ifndef FIX_H
#define FIX_H

#include <stdint.h>

typedef signed int fix15; // 17.15 fixed point

#define FACTOR 32768.0

#define float2fix(a)    ((fix15) (a * FACTOR))
#define fix2float(a)    (((float) a) / FACTOR)
#define int2fix(a)      ((fix15) (((uint32_t) a) << 15))
#define fix2int(a)      ((signed int) a >> 15)
#define multfix(a,b)    ((fix15)(((((signed long long) a))*((signed long long) b)) >> 15))
#define divfix(a,b)     ((fix15)((((signed long long) (a)) << 15) / (b)))
#define square(a)       (multfix(a, a))

#define threehalfs ((fix15) 49152)
#define half       ((fix15) 16384)
#define one        ((fix15) 32768)
#define n_one      ((fix15) -32768)
#define thirty_one ((fix15) 1015808)
#define tentwentythree ((fix15) 33554432)
#define two        ((fix15) 65536)
#define n_two      ((fix15) -65536)
#define four       ((fix15) 131072)

/*
Fixed point approximation of the reciprocal square root of a
return 1/sqrt(a)
*/
fix15 rfixSqrt(fix15 a);

/*
Fixed point approximation of the square root of a 
Uses one divide.
*/
fix15 fixSqrt(fix15 a);

#endif