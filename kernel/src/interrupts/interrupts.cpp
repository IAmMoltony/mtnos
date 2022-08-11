#include <interrupts/interrupts.hpp>
#include <basicrend.hpp>

__attribute__((interrupt)) void page_fault_handler(struct InterruptFrame *frame)
{
    g_renderer->print(0xff00ffff, "Page fault detected!");

    while (true)
        ;
}