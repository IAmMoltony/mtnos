#include <kutil.hpp>

extern "C" void kstart(boot_info_t *bi)
{
    kernel_info_t ki = kernel_init(bi);
    PageTableManager *ptm = ki.ptm;

    g_renderer->set_default_color(0xaaaaaaaa);
    g_renderer->print("Kernel initialized\n");
    
    g_renderer->printf("Hello, %s!\n%d%%\n0x%x", "pizza", 98, 0x1EE7);

    while (true)
        ;
}