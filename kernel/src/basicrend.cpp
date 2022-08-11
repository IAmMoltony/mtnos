#include <basicrend.hpp>

BasicRenderer *g_renderer;

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

void BasicRenderer::vprintf(uint32_t color, const char *format, va_list args)
{
    while (*format)
    {
        switch (*format)
        {
        case '\n':
            cur_pos.x = 0;
            cur_pos.y += 16;
            break;
        case '%':
            ++format;
            switch (*format)
            {
                case 's':
                {
                    char *s = va_arg(args, char *);
                    print(color, s);
                    break;
                }
                case 'd':
                {
                    long d = va_arg(args, long);
                    print_number(color, d);
                    break;
                }
                case 'x':
                {
                    long x = va_arg(args, long);
                    print_hex_number(color, x);
                    break;
                }
                case '%':
                {
                    print(color, "%");
                    break;
                }
            }
            break;
        default:
            putch(color, *format, cur_pos.x, cur_pos.y);
            cur_pos.x += 8;
            if (cur_pos.x + 8 > fb->width)
            {
                cur_pos.x = 0;
                cur_pos.y += 16;
            }
            break;
        }
        ++format;
    }
}

BasicRenderer::BasicRenderer(framebuffer_t *fb, psf1fnt_t *font)
{
    this->fb = fb;
    this->font = font;
    cur_pos = {0, 0};
    default_color = 0xffffffff;
}

void BasicRenderer::set_default_color(uint32_t color)
{
    default_color = color;
}

void BasicRenderer::clear(uint32_t color)
{
    uint64_t fb_base = (uint64_t)fb->base_addr;
    uint64_t bps = fb->px_per_scanline * 4; // bytes per scanline
    uint64_t fb_h = fb->height;
    for (uint64_t i = 0; i < fb_h; ++i)
    {
        uint64_t pixp_base = fb_base + (bps * i);
        for (uint32_t *pixp = (uint32_t *)pixp_base; pixp < (uint32_t *)(pixp_base + bps); ++pixp)
            *pixp = color;
    }
}

void BasicRenderer::print(uint32_t color, const char *str)
{
    while (*str)
    {
        if (*str == '\n')
        {
            cur_pos.x = 0;
            cur_pos.y += 16;
        }
        else
        {
            putch(color, *str, cur_pos.x, cur_pos.y);
            cur_pos.x += 8;
            if (cur_pos.x + 8 > fb->width)
            {
                cur_pos.x = 0;
                cur_pos.y += 16;
            }
        }
        ++str;
    }
}

void BasicRenderer::print(const char *str)
{
    print(default_color, str);
}

void longtostr(long num, char *buf, int base)
{
    int i = 0;
    bool neg = false;

    if (num == 0)
    {
        buf[i] = '0';
        buf[i + 1] = 0;
        return;
    }

    if (num < 0)
    {
        neg = true;
        num *= -1;
    }

    while (num != 0)
    {
        int rem = num % base;
        buf[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num /= base;
    }

    if (neg)
        buf[i++] = '-';
    buf[i] = 0;
    
    for (int start = 0, end = i - 1; start < end; ++start, --end)
    {
        char tmp = *(buf + start);
        *(buf + start) = *(buf + end);
        *(buf + end) = tmp;
    }
}

static char print_num_out[23];
void BasicRenderer::print_number(uint32_t color, long num)
{
    longtostr(num, print_num_out, 10);
    print(color, print_num_out);
}

void BasicRenderer::print_number(long num)
{
    print_number(default_color, num);
}

void BasicRenderer::print_hex_number(uint32_t color, long num)
{
    longtostr(num, print_num_out, 16);
    print(color, print_num_out);
}

void BasicRenderer::print_hex_number(long num)
{
    print_hex_number(default_color, num);
}

void BasicRenderer::printf(uint32_t color, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vprintf(color, format, args);

    va_end(args);
}

void BasicRenderer::printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vprintf(default_color, format, args);

    va_end(args);
}