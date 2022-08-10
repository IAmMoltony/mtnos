#include <paging/paging.h>

void pde_set_flag(page_dir_entry_t *pde, enum ptflag flag, bool enabled)
{
    uint64_t bit_selector = (uint64_t)1 << flag;
    pde->val &= ~bit_selector;
    if (enabled)
        pde->val |= bit_selector;
}

bool pde_get_flag(page_dir_entry_t *pde, enum ptflag flag)
{
    uint64_t bit_selector = (uint64_t)1 << flag;
    return (pde->val & bit_selector) > 0;
}

void pde_set_addr(page_dir_entry_t *pde, uint64_t addr)
{
    addr &= 0x000000ffffffffff;
    pde->val &= 0xfff0000000000fff;
    pde->val |= (addr << 12);
}

uint64_t pde_get_addr(page_dir_entry_t *pde)
{
    return (pde->val & 0x000ffffffffff000) >> 12;
}