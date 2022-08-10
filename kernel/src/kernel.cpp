#include <gop.h>
#include <psf1.h>
#include <efimem.h>
#include <mem.h>
#include <basicrend.hpp>
#include <pageframealloc.hpp>

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
    PageFrameAlloc alloc;
    alloc.read_efi_mmap(bi->mmap, bi->mmapSize, bi->mmapDescSize);

    uint64_t kernel_size = (uint64_t)&_kernelend - (uint64_t)&_kernelstart;
    uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;
    alloc.lock_pages(&_kernelstart, kernel_pages);

    for (int i = 0; i < 28; ++i)
    {
        void *addr = alloc.req_page();
        renderer.print_hex_number((long)addr);
        renderer.print("\n");
    }

    renderer.print("Free memory: ");
    renderer.print_number(get_free_mem() / 1024);
    renderer.print(" KB\nUsed memory: ");
    renderer.print_number(get_used_mem() / 1024);
    renderer.print(" KB\nReserved memory: ");
    renderer.print_number(get_reserved_mem() / 1024);
    renderer.print(" KB\n\n");
}