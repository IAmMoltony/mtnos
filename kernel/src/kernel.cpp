#include <gop.h>
#include <psf1.h>

typedef struct point
{
    uint32_t x, y;
} point_t;

void putch(framebuffer_t *fb, psf1fnt_t *font, uint32_t color, char ch, uint32_t xp, uint32_t yp)
{
    uint32_t *pixp = (uint32_t *)fb->base_addr;
    char *fp = (char *)font->glyphs + (ch * font->psf1hdr->chsize);
    for (uint64_t y = yp; y < yp + 16; ++y)
    {
        for (uint64_t x = xp; x < xp + 8; ++x)
            if ((*fp & (0b10000000 >> (x - xp))) > 0)
                *(uint32_t *)(pixp + x + (y * fb->px_per_scanline)) = color;
        ++fp;
    }
}

point_t cur_pos;
void print(framebuffer_t *fb, psf1fnt_t *font, uint32_t color, const char *str)
{
    while (*str)
    {
        putch(fb, font, color, *str, cur_pos.x, cur_pos.y);
        cur_pos.x += 8;
        if (cur_pos.x + 8 > fb->width)
        {
            cur_pos.x = 0;
            cur_pos.y += 16;
        }
        ++str;
    }
}

extern "C" void kstart(framebuffer_t *fb, psf1fnt_t *font)
{
    cur_pos.x = 0;
    cur_pos.y = 0;
    for (int i = 0; i < 50; ++i)
        print(fb, font, 0xffffffff, "Hello, world!");
}