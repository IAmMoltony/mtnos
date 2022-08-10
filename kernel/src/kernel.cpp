#include <kutil.hpp>

extern "C" void kstart(boot_info_t *bi)
{
    kernel_info_t ki = kernel_init(bi);
    PageTableManager *ptm = ki.ptm;

    BasicRenderer renderer(bi->fb, bi->font);
    renderer.set_default_color(0xaaaaaaaa);

    renderer.print("Kernel initialized\n");

    while (true)
        ;
}