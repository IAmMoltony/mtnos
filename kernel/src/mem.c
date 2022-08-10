#include <mem.h>

uint64_t get_memsize(EFI_MEMORY_DESCRIPTOR *mmap, uint64_t mmap_entries, uint64_t mmap_desc_size)
{
    static uint64_t mem_size = 0;
    if (mem_size > 0)
        return mem_size;
    
    for (uint64_t i = 0; i < mmap_entries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)
            mmap + (i * mmap_desc_size));
        
        mem_size += desc->num_pages * 4096;
    }

    return mem_size;
}