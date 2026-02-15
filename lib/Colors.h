#include <stdint.h>

#define RED     0b1000001111100000
#define BLUE    0b1111110000000000
#define GREEN   0b1000000000011111
#define PURPLE  0b1111111111100000
#define TEAL    0b1111110000011111
#define YELLOW  0b1000001111111111
#define WHITE   0b1111111111111111

#define BMASK   0b0111110000000000
#define RMASK   0b0000001111100000
#define GMASK   0b0000000000011111
#define AMASK   0b1000000000000000

uint16_t RGB_COLOR(uint8_t R, uint8_t G, uint8_t B, bool A)
{
    if(R >= 32 || B >= 32 || G >= 32)
    {
        return 0xFFFF;
    }

    uint16_t res = (A << 15) + ((B << 10) + (R << 5) + G);
    return res;
}
