#ifndef __IDT_HPP__
#define __IDR_HPP__

#include <stdint.h>

#define IDT_TYPE_ATTR_INTERRUPT_GATE 0b10001110
#define IDT_TYPE_ATTR_CALL_GATE 0b10001100
#define IDT_TYPE_ATTR_TRAP_GATE 0b10001111

struct IDTDescEntry
{
    uint16_t off0;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t off1;
    uint32_t off2;
    uint32_t ignored;

    void set_off(uint64_t off);
    uint64_t get_off(void);
};

struct IDTR
{
    uint16_t limit;
    uint64_t off;
} __attribute__((packed));

#endif