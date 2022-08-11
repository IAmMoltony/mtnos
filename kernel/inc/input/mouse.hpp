#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <point.h>
#include <stdint.h>

#define PS2_LMB 0b00000001
#define PS2_MMB 0b00000010
#define PS2_RMB 0b00000100
#define PS2_XSIGN 0b00010000
#define PS2_YSIGN 0b00100000
#define PS2_XOVERFLOW 0b01000000
#define PS2_YOVERFLOW 0b10000000

void ps2mouse_init(void);
void handle_mouse(uint8_t data);
void process_mouse_packet(void);

extern point_t mouse_position;

#endif