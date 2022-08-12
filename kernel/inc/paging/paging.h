#ifndef __PAGING_H__
#define __PAGING_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

enum ptflag
{
    ptfPresent = 0,
    ptfReadWrite = 1,
    ptfUserSuper = 2,
    ptfWriteThrough = 3,
    ptfCacheDisabled = 4,
    ptfAccessed = 5,
    ptfLargerPages = 6,
    ptfCustom0 = 9,
    ptfCustom1 = 10,
    ptfCustom2 = 11,
    ptfNX = 63, // only if supported
};

typedef struct pagedirent
{
    uint64_t val;
} page_dir_entry_t;

void pde_set_flag(page_dir_entry_t *pde, enum ptflag flag, bool enabled);
bool pde_get_flag(page_dir_entry_t *pde, enum ptflag flag);
void pde_set_addr(page_dir_entry_t *pde, uint64_t addr);
uint64_t pde_get_addr(page_dir_entry_t *pde);

typedef struct page_table
{
    page_dir_entry_t entries[512];
} __attribute__((aligned(0x1000))) page_table_t;

#ifdef __cplusplus
}
#endif

#endif