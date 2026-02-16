#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <stdbool.h>

#define GREEN   0b1000001111100000
#define BLUE    0b1111110000000000
#define RED     0b1000000000011111
#define PURPLE  (RED | BLUE) 
#define TEAL    (BLUE | GREEN)
#define YELLOW  (RED | GREEN)
#define WHITE   (RED | BLUE | GREEN) 

#define BMASK   0b0111110000000000
#define GMASK   0b0000001111100000
#define RMASK   0b0000000000011111
#define AMASK   0b1000000000000000

#define RGB_COLOR(R, G, B, A) ((A << 15) + ((B << 10) + (R << 5) + G))

#endif