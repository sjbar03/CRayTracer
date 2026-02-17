#include "Fix.h"

typedef uint16_t raw_color_t;

#define COLOR_FORMAT SDL_PIXELFORMAT_ABGR1555

#define BMASK   0b0111110000000000
#define GMASK   0b0000001111100000
#define RMASK   0b0000000000011111
#define AMASK   0b1000000000000000

/*
Encode three ints (0, 31) as uint16_t ABGR type 
*/
#define RGB_COLOR(R, G, B, A) ((raw_color_t)((A << 15) + ((B << 10) + (G << 5) + R)))

/*
Encode color_t struct as a uint16_t ABGR type
*/
#define ENCODE(C) RGB_COLOR(fix2int(multfix(thirty_one, C.R)), fix2int(multfix(thirty_one, C.G)), fix2int(multfix(thirty_one, C.B)), (int) C.A)