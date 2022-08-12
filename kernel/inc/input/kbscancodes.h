#ifndef __KBSCANCODES_H__
#define __KBSCANCODES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define KEY_LEFT_SHIFT 0x2a
#define KEY_RIGHT_SHIFT 0x36
#define KEY_ENTER 0x1c
#define KEY_BACKSPACE 0x0e
#define KEY_SPACE 0x39

#include <stdint.h>
#include <stdbool.h>

extern const char ascii_table[];
char translate_char(uint8_t scancode, bool upper);

#ifdef __cplusplus
}
#endif

#endif