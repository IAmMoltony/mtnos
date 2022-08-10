#include <paging/pagetablemgr.hpp>
#include <paging/pagemapindexer.hpp>
#include <paging/pageframealloc.hpp>
#include <stdint.h>

PageTableManager::PageTableManager(page_table_t *pml4)
{
    this->pml4 = pml4;
}

void PageTableManager::map_mem(void *virt_addr, void *phys_addr)
{
    PageMapIndexer indexer((uint64_t)virt_addr);
    page_dir_entry_t pde;

    pde = pml4->entries[indexer.pdpi];
    page_table_t *pdp;
    if (!pde_get_flag(&pde, ptfPresent))
    {
        pdp = (page_table_t *)g_pfalloc.req_page();
        memset(pdp, 0, 0x1000);
        pde_set_addr(&pde, (uint64_t)pdp >> 12);
        pde_set_flag(&pde, ptfPresent, true);
        pde_set_flag(&pde, ptfReadWrite, true);
        pml4->entries[indexer.pdpi] = pde;
    }
    else
        pdp = (page_table_t *)((uint64_t)pde_get_addr(&pde) << 12);
    
    pde = pdp->entries[indexer.pdi];
    page_table_t *pd;
    if (!pde_get_flag(&pde, ptfPresent))
    {
        pd = (page_table_t *)g_pfalloc.req_page();
        memset(pd, 0, 0x1000);
        pde_set_addr(&pde, (uint64_t)pd >> 12);
        pde_set_flag(&pde, ptfPresent, true);
        pde_set_flag(&pde, ptfReadWrite, true);
        pdp->entries[indexer.pdi] = pde;
    }
    else
        pd = (page_table_t *)((uint64_t)pde_get_addr(&pde) << 12);
    
    pde = pd->entries[indexer.pti];
    page_table_t *pt;
    if (!pde_get_flag(&pde, ptfPresent))
    {
        pt = (page_table_t *)g_pfalloc.req_page();
        memset(pt, 0, 0x1000);
        pde_set_addr(&pde, (uint64_t)pt >> 12);
        pde_set_flag(&pde, ptfPresent, true);
        pde_set_flag(&pde, ptfReadWrite, true);
        pd->entries[indexer.pti] = pde;
    }
    else
        pt = (page_table_t *)((uint64_t)pde_get_addr(&pde) << 12);
    
    pde = pt->entries[indexer.pi];
    pde_set_addr(&pde, (uint64_t)phys_addr >> 12);
    pde_set_flag(&pde, ptfPresent, true);
    pde_set_flag(&pde, ptfReadWrite, true);
    pt->entries[indexer.pi] = pde;
}