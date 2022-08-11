#include <interrupts/interrupts.hpp>
#include <panic.hpp>

__attribute__((interrupt)) void page_fault_handler(struct InterruptFrame *frame)
{
    panic("Page fault detected");

    while (true)
        ;
}