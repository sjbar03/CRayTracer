#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include "lib/Colors.h"
#include "Viewport.h"
#include <pthread.h>
#include "lib/RayRenderer.h"

SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;
volatile uint16_t gFrameBuffer[WINDOW_HEIGHT * WINDOW_WIDTH];

bool update()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE)
        {
            return false;
        }
    }

    char* pix;
    int pitch;

    SDL_LockTexture(gSDLTexture, NULL, (void **) &pix, &pitch);

    for (int i = 0; i < WINDOW_HEIGHT; i++)
    {
        memcpy(pix + i*pitch, (uint16_t *) gFrameBuffer + i*WINDOW_WIDTH, WINDOW_WIDTH * sizeof(uint16_t));
    }

    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(5);
    return true;
}

void render(uint64_t ticks)
{
    for (int i = 0; i < WINDOW_HEIGHT; i++)
    {
        for (int j = 0; j < WINDOW_WIDTH; j++)
        {
            gFrameBuffer[(i * WINDOW_WIDTH) + j] = RGB_COLOR(31, 0, 31, 1);
        }
    } 
}

void loop()
{
    if (!update())
    {
        gDone = 1;
    }
    else
    {
        // render(SDL_GetTicks());
    }
}

int main(int argc, char** argv)
{
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        return -1;
    }

    gSDLWindow = SDL_CreateWindow("My Window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT); // 16 bit color - I believe

    if(!gSDLWindow || !gSDLRenderer || !gSDLTexture)
    {
        printf("Failed to create SDL components - exiting");
        return -1;
    }
    
    gDone = 0;

    pthread_t thread_ray;
    pthread_create(&thread_ray, NULL, &entry, (void *) &gFrameBuffer);

    while(!gDone)
    {
        loop();
    }

    pthread_kill(thread_ray, 0);

    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();

    return 0;
}