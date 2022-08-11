#ifndef __BASICREND_HPP__
#define __BASICREND_HPP__

#include <gop.h>
#include <psf1.h>
#include <point.h>
#include <stdarg.h>

class BasicRenderer
{
private:
    framebuffer_t *fb;
    psf1fnt_t *font;
    point_t cur_pos;
    uint32_t default_color;

    void putch(uint32_t color, char ch, uint32_t xp, uint32_t yp);
    void vprintf(uint32_t color, const char *format, va_list args);

public:
    BasicRenderer(framebuffer_t *fb, psf1fnt_t *font);

    void set_default_color(uint32_t color);
    void clear(uint32_t color);
    void reset_pos(void);

    void print(uint32_t color, const char *str);
    void print(const char *str);

    void print_number(uint32_t color, long num);
    void print_number(long num);
    
    void print_hex_number(uint32_t color, long num);
    void print_hex_number(long num);

    void printf(uint32_t color, const char *format, ...);
    void printf(const char *format, ...);
};

extern BasicRenderer *g_renderer;

#endif