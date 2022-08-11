#include <kutil.hpp>

extern "C" void kstart(boot_info_t *bi)
{
    kernel_info_t ki = kernel_init(bi);
    PageTableManage
    g_renderer->set_default_color(0xaaaaaaaa);
    g_renderer->clear(0);
    g_renderer->print("Kernel initialized\n");

    while (true)
        ;
}