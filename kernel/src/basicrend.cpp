#include <basicrend.hpp>
#include <input/mouse.hpp>
#include <str.h>

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
                case 'c':
                {
                    int c = va_arg(args, int);
                    putchar(color, c);
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
    clear_color = 0;
}

void BasicRenderer::set_default_color(uint32_t color)
{
    default_color = color;
}

void BasicRenderer::set_clear_color(uint32_t color)
{
    clear_color = color;
}

void BasicRenderer::clear(void)
{
    uint64_t fb_base = (uint64_t)fb->base_addr;
    uint64_t bps = fb->px_per_scanline * 4; // bytes per scanline
    uint64_t fb_h = fb->height;
    for (uint64_t i = 0; i < fb_h; ++i)
    {
        uint64_t pixp_base = fb_base + (bps * i);
        for (uint32_t *pixp = (uint32_t *)pixp_base; pixp < (uint32_t *)(pixp_base + bps); ++pixp)
            *pixp = clear_color;
    }
}

void BasicRenderer::clearch(void)
{
    if (cur_pos.y < 16)
        return;

    if (cur_pos.x == 0)
    {
        cur_pos.x = fb->width;
        cur_pos.y -= 16;
    }
    
    if (cur_pos.x - 8 < 0)
    {
        cur_pos.x = fb->width;
        if (cur_pos.y - 16 > 0)
            cur_pos.y -= 16;
    }
    else
    {
        cur_pos.x -= 8;
    }

    uint32_t *pixp = (uint32_t *)fb->base_addr;
    uint64_t xp = cur_pos.x;
    uint64_t yp = cur_pos.y;
    for (uint64_t y = yp; y < yp + 16; ++y)
        for (uint64_t x = xp; x < xp + 8; ++x)
            *(uint32_t *)(pixp + x + (y * fb->px_per_scanline)) = clear_color;
}

void BasicRenderer::reset_pos(void)
{
    cur_pos = {0, 0};
}

void BasicRenderer::putchar(uint32_t color, char ch)
{
    if (ch == '\n')
    {
        cur_pos.x = 0;
        cur_pos.y += 16;
        return;
    }

    putch(color, ch, cur_pos.x, cur_pos.y);
    cur_pos.x += 8;
    if (cur_pos.x + 8 > fb->width)
    {
        cur_pos.x = 0;
        cur_pos.y += 16;
    }
}

void BasicRenderer::putchar(char ch)
{
    putchar(default_color, ch);
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

void BasicRenderer::putpx(uint32_t x, uint32_t y, uint32_t color)
{
    *(uint32_t *)((uint64_t)fb->base_addr + (x * 4) + (y * fb->px_per_scanline * 4)) = color;
}

static bool mouse_drawn = false;
void BasicRenderer::clear_mouse_cursor(point_t pos)
{
    if (!mouse_drawn)
        return;

    int xm = 16;
    int ym = 16;
    int diffx = fb->width - pos.x;
    int diffy = fb->height - pos.y;

    if (diffx < 16)
        xm = diffx;
    if (diffy < 16)
        ym = diffy;
    
    for (int y = 0; y < ym; ++y)
        for (int x = 0; x < xm; ++x)
        {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouse_pointer[byte] & (0b10000000 >> (x % 8))))
            {
                if (get_px(pos.x + x, pos.y + y) == mouse_cursor_buf_after[x + y * 16])
                    putpx(pos.x + x, pos.y + y, mouse_cursor_buf[x + y * 16]);
            }
        }
}

void BasicRenderer::draw_mouse_cursor(point_t pos, uint32_t color)
{
    int xm = 16;
    int ym = 16;
    int diffx = fb->width - pos.x;
    int diffy = fb->height - pos.y;

    if (diffx < 16)
        xm = diffx;
    if (diffy < 16)
        ym = diffy;
    
    for (int y = 0; y < ym; ++y)
        for (int x = 0; x < xm; ++x)
        {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouse_pointer[byte] & (0b10000000 >> (x % 8))))
            {
                mouse_cursor_buf[x + y * 16] = get_px(pos.x + x, pos.y + y);
                putpx(pos.x + x, pos.y + y, color);
                mouse_cursor_buf_after[x + y * 16] = get_px(pos.x + x, pos.y + y);
            }
        }
    
    mouse_drawn = true;
}

uint32_t BasicRenderer::get_width(void)
{
    return fb->width;
}

uint32_t BasicRenderer::get_height(void)
{
    return fb->height;
}

uint32_t BasicRenderer::get_px(uint32_t x, uint32_t y)
{
    return *(uint32_t *)((uint64_t)fb->base_addr + (x * 4) + (y * fb->px_per_scanline * 4));
}