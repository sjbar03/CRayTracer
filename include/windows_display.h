#include <windows.h>
#include <stdint.h>

#define DEBUG_PRINTF

typedef struct {
    int width;
    int height;
    uint16_t *pixels;
    HWND hwnd;
} Frame; 

extern Frame frame;
void window_init(void);
void draw(void);