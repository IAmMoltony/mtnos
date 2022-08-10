#include <paging/pageframealloc.hpp>

static uint64_t reserved_mem;
static uint64_t free_mem;
static uint64_t used_mem;
static bool init = false;

PageFrameAlloc g_pfalloc;
uint64_t pagebmpi = 0;

void PageFrameAlloc::free_page(void *addr)
{
    uint64_t i = (uint64_t)addr / 4096;
    if (pagebmp[i] == false)
        return; // already free
    if (pagebmp.set(i, false))
    {
        free_mem += 4096;
        used_mem -= 4096;
        if (pagebmpi > i)
            pagebmpi = i;
    }
}

void PageFrameAlloc::lock_page(void *addr)
{
    uint64_t i = (uint64_t)addr / 4096;
    if (pagebmp[i] == true)
        return; // already locked
    if (pagebmp.set(i, true))
    {
        free_mem -= 4096;
        used_mem += 4096;
    }
}

void PageFrameAlloc::unreserve_page(void *addr)
{
    uint64_t i = (uint64_t)addr / 4096;
    if (pagebmp[i] == false)
        return; // already unreserved
    if (pagebmp.set(i, false))
    {
        free_mem += 4096;
        reserved_mem -= 4096;
        if (pagebmpi > i)
            pagebmpi = i;
    }
}

void PageFrameAlloc::reserve_page(void *addr)
{
    uint64_t i = (uint64_t)addr / 4096;
    if (pagebmp[i] == true)
        return; // already reserved
    if (pagebmp.set(i, true))
    {
        free_mem -= 4096;
        reserved_mem += 4096;
    }
}

void PageFrameAlloc::free_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; ++i)
        free_page((void *)((uint64_t)addr + (i * 4096)));
}

void PageFrameAlloc::lock_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; ++i)
        lock_page((void *)((uint64_t)addr + (i * 4096)));
}

void PageFrameAlloc::unreserve_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; ++i)
        unreserve_page((void *)((uint64_t)addr + (i * 4096)));
}

void PageFrameAlloc::reserve_pages(void *addr, uint64_t count)
{
    for (uint64_t i = 0; i < count; ++i)
        reserve_page((void *)((uint64_t)addr + (i * 4096)));
}

void *PageFrameAlloc::req_page(void)
{
    for (; pagebmpi < pagebmp.size * 8; ++pagebmpi)
    {
        if (pagebmp[pagebmpi] == true) // not free
            continue;
        
        lock_page((void *)(pagebmpi * 4096));
        return (void *)(pagebmpi * 4096);
    }

    return 0; // cant find a page
}

void PageFrameAlloc::read_efi_mmap(EFI_MEMORY_DESCRIPTOR *mmap, size_t mmap_size, size_t mmap_desc_size)
{
    if (init)
        return;
    init = true;

    uint64_t mmap_entries = mmap_size / mmap_desc_size;
    void *largest_free = NULL;
    size_t largest_free_size = 0;
    for (uint64_t i = 0; i < mmap_entries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)mmap + (i * mmap_desc_size));
        if (desc->type == 7) // conventional memory
        {
            if (desc->num_pages * 4096 > largest_free_size)
            {
                largest_free = desc->phys_addr;
                largest_free_size = desc->num_pages * 4096;
            }
        }
    }

    uint64_t mem_size = get_memsize(mmap, mmap_entries, mmap_desc_size);
    free_mem = mem_size;
    uint64_t bmpsize = mem_size / 4096 / 8 + 1;

    init_bmp(bmpsize, largest_free);
    lock_pages(&pagebmp, pagebmp.size / 4096 + 1);

    for (uint64_t i = 0; i < mmap_entries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)mmap + (i * mmap_desc_size));
        if (desc->type != 7) // not conventional memory
            reserve_pages(desc->phys_addr, desc->num_pages);
    }
}

void PageFrameAlloc::init_bmp(size_t bmpsize, void *buf_addr)
{
    pagebmp.size = bmpsize;
    pagebmp.buf = (uint8_t *)buf_addr;
    for (uint64_t i = 0; i < bmpsize; ++i)
    {
        *(uint8_t *)(pagebmp.buf + i) = 0;
    }
}

uint64_t get_free_mem(void)
{
    return free_mem;
}

uint64_t get_used_mem(void)
{
    return used_mem;
}

uint64_t get_reserved_mem(void)
{
    return reserved_mem;
}