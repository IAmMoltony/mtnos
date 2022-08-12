#ifndef __EFIMEM_H__
#define __EFIMEM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct efi_memdesc
{
    uint32_t type;
    void *phys_addr;
    void *virt_addr;
    uint64_t num_pages;
    uint64_t attribs;
} EFI_MEMORY_DESCRIPTOR;

extern const char *EFI_MEMORY_TYPE_STRINGS[];

#ifdef __cplusplus
}
#endif

#endif