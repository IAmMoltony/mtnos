#ifndef __INTERRUPTS_HPP__
#define __INTERRUPTS_HPP__

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1
#define PIC_EOI 0x20
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct InterruptFrame;
__attribute__((interrupt)) void page_fault_handler(struct InterruptFrame *frame);
__attribute__((interrupt)) void double_fault_handler(struct InterruptFrame *frame);
__attribute__((interrupt)) void gp_fault_handler(struct InterruptFrame *frame);
__attribute__((interrupt)) void keyboard_int_handler(struct InterruptFrame *frame);
__attribute__((interrupt)) void mouse_int_handler(struct InterruptFrame *frame);

// gp stands for general protection

void remap_pic(void);
void pic_endmaster(void);
void pic_endslave(void);

#endif