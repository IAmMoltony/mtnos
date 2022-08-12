#include <panic.hpp>
#include <basicrend.hpp>

bool g_panic_fullscreen = true;

void panic(const char *msg)
{
    g_renderer->set_default_color(0xffffffff);

    if (g_panic_fullscreen)
    {
        g_renderer->set_clear_color(0x00ff0000);
        g_renderer->clear();
        g_renderer->reset_pos();
    }

    g_renderer->printf("Kernel panic!\nMessage: %s\n\n", msg);
}