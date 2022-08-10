#ifndef __MEM_H__
#define __MEM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <efimem.h>
uint64_t get_memsize(EFI_MEMORY_DESCRIPTOR *mmap, uint64_t mmap_entries, uint64_t mmap_desc_size);

#ifdef __cplusplus
}
#endif

#endif