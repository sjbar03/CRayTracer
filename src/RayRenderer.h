#ifndef RAYRENDERER_H
#define RAYRENDERER_H

#include <stdint.h>
#include <signal.h>

extern volatile sig_atomic_t gDone;

void *entry(void *frame_buffer);

#endif
