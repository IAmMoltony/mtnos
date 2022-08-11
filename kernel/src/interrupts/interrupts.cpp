#include <interrupts/interrupts.hpp>
#include <panic.hpp>
#include <basicrend.hpp>
#include <io.h>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>

__attribute__((interrupt)) void page_fault_handler(struct InterruptFrame *frame)
{
    panic("Page fault detected");

    while (true)
        ;
}

__attribute__((interrupt)) void double_fault_handler(struct InterruptFrame *frame)
{
    panic("Double fault detected");

    while (true)
        ;
}

__attribute__((interrupt)) void gp_fault_handler(struct InterruptFrame *frame)
{
    panic("General protection fault detected");

    while (true)
        ;
}

__attribute__((interrupt)) void keyboard_int_handler(struct InterruptFrame *frame)
{
    uint8_t scancode = inb(0x60);
    handle_kb(scancode);
    pic_endmaster();
}

__attribute__((interrupt)) void mouse_int_handler(struct InterruptFrame *frame)
{
    uint8_t data = inb(0x60);
    handle_mouse(data);
    pic_endslave();
}

void remap_pic(void)
{
    uint8_t a1, a2;
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}

void pic_endmaster(void)
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_endslave(void)
{
    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC2_COMMAND, PIC_EOI);
}