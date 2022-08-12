#ifndef __PAGEMAPINDEXER_HPP__
#define __PAGEMAPINDEXER_HPP__

#include <stdint.h>

class PageMapIndexer
{
public:
    uint64_t pdpi;
    uint64_t pdi;
    uint64_t pti;
    uint64_t pi;

    PageMapIndexer(uint64_t virt_addr);
};

#endif