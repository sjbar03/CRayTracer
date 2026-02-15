#include "Fix.h"

#define threehalfs ((fix15) 49152)
#define half       ((fix15) 16384)
#define PRECISION  5

fix15 rfixSqrt(fix15 a)
{
    if (a <= 0) return 0;

    // Normalize a into [0.5, 2.0) by factoring out powers of 4.
    // rSqrt(a) = rSqrt(a_norm) >> k   where a = a_norm * 4^k
    int k = 0;
    fix15 a_norm = a;

    while (a_norm >= int2fix(2))
    {
        a_norm >>= 2;
        k++;
    }

    while (a_norm < float2fix(0.5))
    {
        a_norm <<= 2;
        k--;
    }

    // Initial guess of 1.0 works well for a_norm in [0.5, 2.0)
    fix15 yn = int2fix(1);
    fix15 yn2, half_ayn2;

    for (int i = 0; i < PRECISION; i++)
    {
        yn2 = square(yn);
        half_ayn2 = multfix(half, multfix(a_norm, yn2));
        yn = multfix(yn, threehalfs - half_ayn2);
    }

    // Scale result back
    if (k > 0)
        yn >>= k;
    else if (k < 0)
        yn <<= (-k);

    return yn;
}

fix15 fixSqrt(fix15 a)
{
    return divfix(int2fix(1), rfixSqrt(a));
}