#ifndef __GOP_H__
#define __GOP_H__

#include <stdint.h>
#include <stddef.h>

typedef struct framebuffer
{
    void *base_addr;
    size_t size;
    uint32_t width;
    uint32_t height;
    uint32_t px_per_scanline;
} framebuffer_t;

#endif