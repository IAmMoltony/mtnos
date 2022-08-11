#include <interrupts/idt.hpp>

void IDTDescEntry::set_off(uint64_t off)
{
    off0 = (uint16_t)(off & 0x000000000000ffff);
    off1 = (uint16_t)((off & 0x00000000ffff0000) >> 16);
    off2 = (uint32_t)((off & 0xffffffff00000000) >> 32);
}

uint64_t IDTDescEntry::get_off(void)
{
    uint64_t off = 0;
    off |= (uint64_t)off0;
    off |= (uint64_t)off1 << 16;
    off |= (uint64_t)off2 << 32;
    return off;
}