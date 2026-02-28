#include "Fix.h"

typedef uint32_t raw_color_t;

#define COLOR_FORMAT SDL_PIXELFORMAT_ARGB2101010

#define RGB_COLOR(R, G, B, A) ((raw_color_t) (((A * 3) << 30) + ((R << 20) + (G << 10) + B)))

#define ENCODE(C) RGB_COLOR(fix2int(multfix(tentwentythree - one, C.R)), fix2int(multfix(tentwentythree - one, C.G)), fix2int(multfix(tentwentythree - one, C.B)), (int) C.A)