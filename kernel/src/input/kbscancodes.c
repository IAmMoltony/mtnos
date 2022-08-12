#include <input/kbscancodes.h>

const char ascii_table[] =
{
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c',
    'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
};

char translate_char(uint8_t scancode, bool upper)
{
    if (scancode > 58)
        return 0; // too big scancode
    
    if (upper)
        return ascii_table[scancode] - 32;
    return ascii_table[scancode];
}