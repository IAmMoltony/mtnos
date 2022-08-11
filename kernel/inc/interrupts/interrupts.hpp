#ifndef __INTERRUPTS_HPP__
#define __INTERRUPTS_HPP__

struct InterruptFrame;
__attribute__((interrupt)) void page_fault_handler(struct InterruptFrame *frame);
__attribute__((interrupt)) void double_fault_handler(struct InterruptFrame *frame);
__attribute__((interrupt)) void gp_fault_handler(struct InterruptFrame *frame);

// gp stands for general protection

#endif