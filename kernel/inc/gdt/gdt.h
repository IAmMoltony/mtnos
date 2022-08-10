#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

typedef struct gdt_desc
{
    uint16_t size;
    uint64_t off;
} __attribute__((packed)) gdt_desc_t;

typedef struct gdt_entry
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access_byte;
    uint8_t limit1_flags;
    uint8_t base2;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt
{
    gdt_entry_t null;
    gdt_entry_t kernel_code;
    gdt_entry_t kernel_data;
    gdt_entry_t user_null;
    gdt_entry_t user_code;
    gdt_entry_t user_data;
} __attribute__((packed)) __attribute__((aligned(0x1000))) gdt_t;

extern gdt_t gdt_default;

extern
#ifdef __cplusplus
"C"
#endif
void load_gdt(gdt_desc_t *gdtdesc);

#endif