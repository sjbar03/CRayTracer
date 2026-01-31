#include "windows_display.h"
#include <pthread.h>



void * graphics(void *arg)
{
    (void) arg; // compiler silencing
    for(;;)
    {
        static unsigned int p = 0;
        // frame.pixels[(p++)%(frame.width*frame.height)] = rand();
        // frame.pixels[rand()%(frame.width*frame.height)] = 0;
        int n = frame.width * frame.height;
        if (frame.pixels && n > 0) {
            frame.pixels[(p++) % n] = (uint16_t)rand();
            frame.pixels[rand() % n] = 0;
        } 

        draw();   
    } 


    return NULL;
}

void * window(void *arg)
{
    (void) arg; // compiler silencing
    window_init();
    return NULL;
}

int main()
{
    pthread_t thread_graphics;
    pthread_t thread_window;

    pthread_create(&thread_window, NULL, &window, NULL);
    pthread_create(&thread_graphics, NULL, &graphics, NULL);

    pthread_join(thread_graphics, NULL);
    pthread_join(thread_window, NULL);
}