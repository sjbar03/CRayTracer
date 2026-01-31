#include <windows.h>
#include <stdint.h>

typedef struct {
    int width;
    int height;
    uint16_t *pixels;
    HWND hwnd;
} Frame; 

extern Frame frame;
void window_init(void);
void draw(void);