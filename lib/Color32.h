#ifndef COLOR_TYPE
#define COLOR_TYPE

#include "Fix.h"

typedef uint32_t raw_color_t;

#define COLOR_FORMAT SDL_PIXELFORMAT_RGBA8888 

#define RGB_COLOR(R, G, B, A) (raw_color_t) (((uint32_t) R) << 24) | (((uint32_t) G) << 16) | (((uint32_t) B) << 8) | ((uint32_t) A)

#define ENCODE(C) RGB_COLOR(fix2int(multfix(int2fix(255), C.R)), fix2int(multfix(int2fix(255), C.G)), fix2int(multfix(int2fix(255), C.B)), 255 * (int) C.A )

#endif