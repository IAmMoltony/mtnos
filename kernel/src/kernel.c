#include <gop.h>
#include <psf1.h>

void putch(framebuffer_t *fb, psf1fnt_t *font, uint32_t color, char ch, uint32_t xp, uint32_t yp)
{
    uint32_t *pixp = (uint32_t *)fb->base_addr;
    char *fp = font->glyphs + (ch * font->psf1hdr->chsize);
    for (uint64_t y = yp; y < yp + 16; ++y)
    {
        for (uint64_t x = xp; x < xp + 8; ++x)
            if ((*fp & (0b10000000 >> (x - xp))) > 0)
                *(uint32_t *)(pixp + x + (y * fb->px_per_scanline)) = color;
        ++fp;
    }
}

void kstart(framebuffer_t *fb, psf1fnt_t *font)
{
    putch(fb, font, 0xffffffff, 'r', 10, 10);
}