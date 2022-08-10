#include <gop.h>
#include <psf1.h>
#include <efimem.h>
#include <mem.h>
#include <basicrend.hpp>
#include <paging/pageframealloc.hpp>
#include <paging/pagemapindexer.hpp>
#include <paging/pagetablemgr.hpp>

typedef struct boot_info
{
	framebuffer_t *fb;
	psf1fnt_t *font;
	EFI_MEMORY_DESCRIPTOR *mmap;
	uint64_t mmapSize;
	uint64_t mmapDescSize;
} boot_info_t;

extern uint64_t _kernelstart;
extern uint64_t _kernelend;

extern "C" void kstart(boot_info_t *bi)
{
    BasicRenderer renderer(bi->fb, bi->font);
    renderer.set_default_color(0xaaaaaaaa);

    uint64_t mmap_entries = bi->mmapSize / bi->mmapDescSize;
    g_pfalloc = PageFrameAlloc();
    g_pfalloc.read_efi_mmap(bi->mmap, bi->mmapSize, bi->mmapDescSize);

    uint64_t kernel_size = (uint64_t)&_kernelend - (uint64_t)&_kernelstart;
    uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;
    g_pfalloc.lock_pages(&_kernelstart, kernel_pages);

    page_table_t *pml4 = (page_table_t *)g_pfalloc.req_page();
    memset(pml4, 0, 0x1000);
    PageTableManager ptm(pml4);

    for (uint64_t i = 0; i < get_memsize(bi->mmap, mmap_entries, bi->mmapDescSize); i += 0x1000)
    {
        ptm.map_mem((void *)i, (void *)i);
    }
    uint64_t fb_base = (uint64_t)bi->fb->base_addr;
    uint64_t fb_size = (uint64_t)bi->fb->size + 0x1000;
    for (uint64_t i = fb_base; i < fb_base +fb_size; i += 0x1000)
    {
        ptm.map_mem((void *)i, (void *)i);
    }

    asm("mov %0, %%cr3" : : "r"(pml4));
}