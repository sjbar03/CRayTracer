#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <stdbool.h>
#include "Fix.h"
#include "Color32.h" // Change to switch color lib

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
#define lightgray ((color_t) {true, float2fix(0.1),float2fix(0.1),float2fix(0.1)})
#define skyblue ((color_t) {true, float2fix(0.4), float2fix(0.60), float2fix(1.0)})
#define babyblue ((color_t) {true, float2fix(0.27), float2fix(0.405), float2fix(0.7)})
#define paleblue ((color_t) {true, float2fix(0.33), float2fix(0.40), float2fix(0.60)})

color_t linear_interpolate_color(color_t C1, color_t C2, fix15 t);
color_t color_scale(color_t in, fix15 scale);
color_t add_color(color_t in1, color_t in2);
color_t gamma_correct(color_t c);

#endif