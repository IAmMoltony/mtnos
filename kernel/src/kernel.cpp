#include <gop.h>
#include <psf1.h>
#include <basicrend.hpp>

extern "C" void kstart(framebuffer_t *fb, psf1fnt_t *font)
{
    BasicRenderer renderer(fb, font);
    renderer.print_number(-1234567890);
}