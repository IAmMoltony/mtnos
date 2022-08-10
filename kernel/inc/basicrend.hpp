#ifndef __BASICREND_HPP__
#define __BASICREND_HPP__

#include <gop.h>
#include <psf1.h>
#include <point.h>

class BasicRenderer
{
private:
    framebuffer_t *fb;
    psf1fnt_t *font;
    point_t cur_pos;

    void putch(uint32_t color, char ch, uint32_t xp, uint32_t yp);

public:
    BasicRenderer(framebuffer_t *fb, psf1fnt_t *font);

    void print(uint32_t color, const char *str);
    void print(const char *str);

    void print_number(uint32_t color, long num);
    void print_number(long num);
    
    void print_hex_number(uint32_t color, long num);
    void print_hex_number(long num);
};

#endif