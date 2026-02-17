#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <stdbool.h>
#include "Fix.h"

typedef struct 
{
    bool A;
    fix15 R;
    fix15 G;
    fix15 B;
} color_t ;

#define green   ((color_t) {true, 0, one, 0})
#define red     ((color_t) {true, one, 0, 0})
#define blue    ((color_t) {true, 0, 0, one})
#define purple  ((color_t) {true, one, 0, one})
#define teal    ((color_t) {true, 0, one, one})
#define yellow  ((color_t) {true, one, one, 0})
#define white   ((color_t) {true, one, one, one})
#define black   ((color_t) {true, 0, 0, 0})
#define gray    ((color_t) {true, half, half, half})
#define skyblue ((color_t) {true, float2fix(0.529), float2fix(0.808), float2fix(0.922)})

#define BMASK   0b0111110000000000
#define GMASK   0b0000001111100000
#define RMASK   0b0000000000011111
#define AMASK   0b1000000000000000

/*
Encode three ints (0, 31) as uint16_t ABGR type 
*/
#define RGB_COLOR(R, G, B, A) ((A << 15) + ((B << 10) + (G << 5) + R))

/*
Encode color_t struct as a uint16_t ABGR type
*/
#define ENCODE(C) RGB_COLOR(fix2int(multfix(thirty_one, C.R)), fix2int(multfix(thirty_one, C.G)), fix2int(multfix(thirty_one, C.B)), (int) C.A)

#endif