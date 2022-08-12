#ifndef __PAGETABLEMGR_HPP__
#define __PAGETABLEMGR_HPP__

#include <paging/paging.h>

class PageTableManager
{
public:
    page_table_t *pml4;

    PageTableManager(page_table_t *pml4);
    void map_mem(void *virt_addr, void *phys_addr);
};

extern PageTableManager g_ptm;

#endif