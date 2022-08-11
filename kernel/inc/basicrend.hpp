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
    uint32_t clear_color;
    uint32_t mouse_cursor_buf[16 * 16];
    uint32_t mouse_cursor_buf_after[16 * 16];

    void vprintf(uint32_t color, const char *format, va_list args);

public:
    BasicRenderer(framebuffer_t *fb, psf1fnt_t *font);

    void set_default_color(uint32_t color);
    void set_clear_color(uint32_t color);
    void clear(void);
    void clearch(void);
    void reset_pos(void);

    void putch(uint32_t color, char ch, uint32_t xp, uint32_t yp);
    void putpx(uint32_t x, uint32_t y, uint32_t color);
    void putchar(uint32_t color, char ch);
    void putchar(char ch);

    void print(uint32_t color, const char *str);
    void print(const char *str);

    void print_number(uint32_t color, long num);
    void print_number(long num);
    
    void print_hex_number(uint32_t color, long num);
    void print_hex_number(long num);

    void printf(uint32_t color, const char *format, ...);
    void printf(const char *format, ...);

    void clear_mouse_cursor(point_t pos);
    void draw_mouse_cursor(point_t pos, uint32_t color);

    uint32_t get_width(void);
    uint32_t get_height(void);
    uint32_t get_px(uint32_t x, uint32_t y);
};

extern BasicRenderer *g_renderer;

#endif