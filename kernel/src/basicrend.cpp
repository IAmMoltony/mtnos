#include <basicrend.hpp>

void BasicRenderer::putch(uint32_t color, char ch, uint32_t xp, uint32_t yp)
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

BasicRenderer::BasicRenderer(framebuffer_t *fb, psf1fnt_t *font)
{
    this->fb = fb;
    this->font = font;
    cur_pos = {0, 0};
}

void BasicRenderer::print(uint32_t color, const char *str)
{
    while (*str)
    {
        putch(color, *str, cur_pos.x, cur_pos.y);
        cur_pos.x += 8;
        if (cur_pos.x + 8 > fb->width)
        {
            cur_pos.x = 0;
            cur_pos.y += 16;
        }
        ++str;
    }
}

void BasicRenderer::print(const char *str)
{
    print(0xffffffff, str);
}

static char ntos_out[23]; // i think 23 is enough
void BasicRenderer::print_number(uint32_t color, long num)
{
    if (num < 0)
    {
        print(color, "-");
        num *= -1;
    }

    uint8_t size;
    uint64_t size_test = num;
    while (size_test / 10 > 0)
    {
        size_test /= 10;
        ++size;
    }

    uint8_t i = 0;
    while (num / 10 > 0)
    {
        uint8_t rem = num % 10;
        num /= 10;
        ntos_out[size - i] = rem + '0';
        ++i;
    }

    uint8_t rem = num % 10;
    ntos_out[size - i] = rem + '0';
    ntos_out[size + 1] = 0;
    print(color, ntos_out);
}

void BasicRenderer::print_number(long num)
{
    print_number(0xffffffff, num);
}