#include <input/keyboard.hpp>
#include <input/kbscancodes.h>
#include <basicrend.hpp>

static bool left_shift = false;
static bool right_shift = false;
void handle_kb(uint8_t scancode)
{
    switch (scancode)
    {
    case KEY_SPACE:
        g_renderer->putchar(' ');
        return;
    case KEY_LEFT_SHIFT: // left shift presed
        left_shift = true;
        return;
    case KEY_LEFT_SHIFT + 0x80: // left shift released
        left_shift = false;
        return;
    case KEY_RIGHT_SHIFT: // right shift pressed
        right_shift = true;
        return;
    case KEY_RIGHT_SHIFT + 0x80:
        left_shift = false;
        return;
    case KEY_ENTER:
        g_renderer->putchar('\n');
        return;
    case KEY_BACKSPACE:
        g_renderer->clearch();
        return;
    }

    char ascii = translate_char(scancode, left_shift || right_shift);
    if (ascii != 0)
        g_renderer->putchar(ascii);
}