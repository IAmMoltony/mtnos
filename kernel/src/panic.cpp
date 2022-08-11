#include <panic.hpp>
#include <basicrend.hpp>

void panic(const char *msg)
{
    g_renderer->clear(0x00ff0000);
    g_renderer->set_default_color(0xffffffff);
    g_renderer->reset_pos();

    g_renderer->printf("Kernel panic!\n\nMessage: %s\n", msg);
}