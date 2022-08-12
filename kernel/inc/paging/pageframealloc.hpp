#ifndef __PAGEFRAMEALLOC_HPP__
#define __PAGEFRAMEALLOC_HPP__

#include <efimem.h>
#include <stdint.h>
#include <bitmap.hpp>
#include <mem.h>

class PageFrameAlloc
{
private:
    void init_bmp(size_t bmpsize, void *buf_addr);

    void reserve_page(void *addr);
    void reserve_pages(void *addr, uint64_t count);
    
    void unreserve_page(void *addr);
    void unreserve_pages(void *addr, uint64_t count);

public:
    Bitmap pagebmp;

    void free_page(void *addr);
    void free_pages(void *addr, uint64_t count);

    void lock_page(void *addr);
    void lock_pages(void *addr, uint64_t count);

    void *req_page(void);

    void read_efi_mmap(EFI_MEMORY_DESCRIPTOR *mmap, size_t mmap_size, size_t mmap_desc_size);
};

uint64_t get_free_mem(void);
uint64_t get_used_mem(void);
uint64_t get_reserved_mem(void);

extern PageFrameAlloc g_pfalloc;

#endif