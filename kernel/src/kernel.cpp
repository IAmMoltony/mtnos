#include <kutil.hpp>
#include <input/mouse.hpp>
#include <str.h>

extern "C" void kstart(boot_info_t *bi)
{
    kernel_info_t ki = kernel_init(bi);
    PageTableManager *ptm = ki.ptm;
    g_renderer->print("Kernel initialized\n");
    g_renderer->printf("%x\n", (uint64_t)bi->rsdp);
    for (uint8_t i = 0; i < 8; ++i)
    {
        g_renderer->putchar(*((uint8_t *)bi->rsdp + i));
    }
    g_renderer->printf("\n%d", strlen("hello"));
    g_renderer->print("\n\n");

    while (true)
    {
        process_mouse_packet();
    }

    while (true)
        ;
}