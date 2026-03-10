#include "Colors.h"

color_t linear_interpolate_color(color_t C1, color_t C2, fix15 t)
{
    fix15 j = one - t;
    color_t res;

    res.R = multfix(t, C2.R) + multfix(j, C1.R);
    res.G = multfix(t, C2.G) + multfix(j, C1.G);
    res.B = multfix(t, C2.B) + multfix(j, C1.B);
    res.A = 1;

    return res;
}

color_t color_scale(color_t in, fix15 scale)
{
    return (color_t) {
        true,
        multfix(in.R, scale),
        multfix(in.G, scale),
        multfix(in.B, scale)
    };
}


color_t add_color(color_t in1, color_t in2)
{
    return (color_t) {
        true,
        in1.R + in2.R,
        in1.G + in2.G,
        in1.B + in2.B
    };
}

color_t gamma_correct(color_t c)
{
    if (c.R < 0) c.R = 0;
    if (c.G < 0) c.G = 0;
    if (c.B < 0) c.B = 0;

    if (c.R > one) c.R = one;
    if (c.G > one) c.G = one;
    if (c.B > one) c.B = one;

    c.R = fixSqrt(c.R);
    c.G = fixSqrt(c.G);
    c.B = fixSqrt(c.B);

    return c;
}