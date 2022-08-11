#include <interrupts/interrupts.hpp>
#include <panic.hpp>

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