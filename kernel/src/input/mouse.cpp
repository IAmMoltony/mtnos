#include <input/mouse.hpp>
#include <io.h>
#include <stdbool.h>
#include <basicrend.hpp>

uint8_t mouse_pointer[] =
{
    0b11000000, 0b00000000,
    0b11100000, 0b00000000,
    0b11110000, 0b00000000,
    0b11111000, 0b00000000,
    0b11111100, 0b00000000,
    0b11111110, 0b00000000,
    0b11111111, 0b00000000,
    0b11111111, 0b10000000,
    0b11111110, 0b00000000,
    0b11101111, 0b00000000,
    0b11001111, 0b00000000,
    0b00000111, 0b10000000,
    0b00000111, 0b10000000,
    0b00000011, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
};

void mouse_wait(void)
{
    uint64_t timeout = 100000;
    while (timeout--)
    {
        if ((inb(0x64) & 0b10) == 0)
            return;
    }
}

void mouse_wait_inp(void)
{
    uint64_t timeout = 100000;
    while (timeout--)
    {
        if (inb(0x64) & 0b1)
            return;
    }
}

void mouse_write(uint8_t val)
{
    mouse_wait();
    outb(0x64, 0xd4); // about to send command
    mouse_wait();
    outb(0x60, val);
}

uint8_t mouse_read(void)
{
    mouse_wait_inp();
    return inb(0x60);
}

uint8_t mouse_cycle = 0;
uint8_t mouse_packet[4];
bool mouse_packet_ready = false;
point_t mouse_position;
point_t mouse_position_old;
void handle_mouse(uint8_t data)
{
    process_mouse_packet();

    // first packet out of sync, skip it
    static bool skip = true;
    if (skip)
    {
        skip = false;
        return;
    }

    switch (mouse_cycle)
    {
    case 0:
        if ((data & 0b00001000) == 0)
            break;
        mouse_packet[0] = data;
        ++mouse_cycle;
        break;
    case 1:
        mouse_packet[1] = data;
        ++mouse_cycle;
        break;
    case 2:
        mouse_packet[2] = data;
        mouse_packet_ready = true;
        mouse_cycle = 0;
        break;
    }
}

void process_mouse_packet(void)
{
    if (!mouse_packet_ready)
        return;
    
    bool x_neg, y_neg, x_overflow, y_overflow;
    x_neg = (mouse_packet[0] & PS2_XSIGN);
    y_neg = (mouse_packet[0] & PS2_YSIGN);
    x_overflow = (mouse_packet[0] & PS2_XOVERFLOW);
    y_overflow = (mouse_packet[0] & PS2_YOVERFLOW);

    if (!x_neg)
    {
        mouse_position.x += mouse_packet[1];
        if (x_overflow)
            mouse_position.x += 255;
    }
    else
    {
        mouse_packet[1] = 256 - mouse_packet[1];
        mouse_position.x -= mouse_packet[1];
        if (x_overflow)
            mouse_position.x -= 255;
    }

    if (!y_neg)
    {
        mouse_position.y -= mouse_packet[2];
        if (y_overflow)
            mouse_position.y -= 255;
    }
    else
    {
        mouse_packet[2] = 256 - mouse_packet[2];
        mouse_position.y += mouse_packet[2];
        if (y_overflow)
            mouse_position.y += 255;
    }

    if (mouse_position.x < 0)
        mouse_position.x = 0;
    if (mouse_position.x > g_renderer->get_width() - 9)
        mouse_position.x = g_renderer->get_width() - 1;
    if (mouse_position.y < 0)
        mouse_position.y = 0;
    if (mouse_position.y > g_renderer->get_height() - 17)
        mouse_position.y = g_renderer->get_height() - 1;

    g_renderer->clear_mouse_cursor(mouse_position_old);
    g_renderer->draw_mouse_cursor(mouse_position, 0x0000bb00);

    if (mouse_packet[0] & PS2_LMB)
        g_renderer->putch(0x00ff0000, '#', mouse_position.x, mouse_position.y);
    if (mouse_packet[0] & PS2_MMB)
        g_renderer->putch(0x0000ff00, '#', mouse_position.x, mouse_position.y);
    if (mouse_packet[0] & PS2_RMB)
        g_renderer->putch(0x000000ff, '#', mouse_position.x, mouse_position.y);

    mouse_packet_ready = false;
    mouse_position_old = mouse_position;
}

void ps2mouse_init(void)
{
    outb(0x64, 0xa8); // enable aux device mouse
    mouse_wait();
    outb(0x64, 0x20); // tell keyboard controllet that we want to send command to mouse
    mouse_wait_inp();
    uint8_t status = inb(0x60);
    status |= 0b10;
    mouse_wait();
    outb(0x64, 0x60); // tell that we are about to send command
    mouse_wait();
    outb(0x60, status); // setting correct bit is the compaq status byte
    mouse_write(0xf6); // default mouse settings
    mouse_read();
    mouse_write(0xf4);
    mouse_read();
}