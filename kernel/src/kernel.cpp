#include <gop.h>
#include <psf1.h>
#include <efimem.h>
#include <mem.h>
#include <basicrend.hpp>

typedef struct boot_info
{
	framebuffer_t *fb;
	psf1fnt_t *font;
	EFI_MEMORY_DESCRIPTOR *mmap;
	uint64_t mmapSize;
	uint64_t mmapDescSize;
} boot_info_t;

extern "C" void kstart(boot_info_t *bi)
{
    BasicRenderer renderer(bi->fb, bi->font);
    renderer.set_default_color(0xaaaaaaaa);

    uint64_t mmap_entries = bi->mmapSize / bi->mmapDescSize;
    uint64_t mem_size = get_memsize(bi->mmap, mmap_entries, bi->mmapDescSize);
    renderer.print("Memory size: ");
    renderer.print_number(0x0000ff00, mem_size / 1024);
    renderer.print(0x0000ff00, " KB\n\n");

    for (uint64_t i = 0; i < mmap_entries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)
            bi->mmap + (i * bi->mmapDescSize));
        
        renderer.print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
        renderer.print(" - ");
        renderer.print_number(0x0000ff00, desc->num_pages * 4096 / 1024);
        renderer.print(0x0000ff00, " KB");
        renderer.print("\n");
    }
}