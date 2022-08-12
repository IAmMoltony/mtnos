#include <kutil.hpp>
#include <input/mouse.hpp>
<<<<<<< HEAD
#include <str.h>
=======
>>>>>>> 2b1eebe50b87486f84251295194968295669c374

extern "C" void kstart(boot_info_t *bi)
{
    kernel_info_t ki = kernel_init(bi);
    PageTableManager *ptm = ki.ptm;
<<<<<<< HEAD
    g_renderer->print("Kernel initialized\n");
    g_renderer->printf("%x\n", (uint64_t)bi->rsdp);
    for (uint8_t i = 0; i < 8; ++i)
    {
        g_renderer->putchar(*((uint8_t *)bi->rsdp + i));
    }
    g_renderer->printf("\n%d", strlen("hello"));
    g_renderer->print("\n\n");
=======
    g_renderer->set_default_color(0xaaaaaaaa);
    g_renderer->set_clear_color(0);
    g_renderer->clear();
    g_renderer->print("Kernel initialized\n\n");
>>>>>>> 2b1eebe50b87486f84251295194968295669c374

    while (true)
    {
        process_mouse_packet();
    }

    while (true)
        ;
}