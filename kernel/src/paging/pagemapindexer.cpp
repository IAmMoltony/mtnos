#include <paging/pagemapindexer.hpp>

PageMapIndexer::PageMapIndexer(uint64_t virt_addr)
{
    virt_addr >>= 12;
    pi = virt_addr & 0x1ff;
    virt_addr >>= 9;
    pti = virt_addr & 0x1ff;
    virt_addr >>= 9;
    pdi = virt_addr & 0x1ff;
    virt_addr >>= 9;
    pdpi = virt_addr & 0x1ff;
}