#ifndef __KUTIL_HPP__
#define __KUTIL_HPP__

#include <gop.h>
#include <psf1.h>
#include <efimem.h>
#include <mem.h>
#include <basicrend.hpp>
#include <paging/pageframealloc.hpp>
#include <paging/pagemapindexer.hpp>
#include <paging/pagetablemgr.hpp>
<<<<<<< HEAD
#include <acpi.h>
=======
>>>>>>> 2b1eebe50b87486f84251295194968295669c374

typedef struct boot_info
{
	framebuffer_t *fb;
	psf1fnt_t *font;
	EFI_MEMORY_DESCRIPTOR *mmap;
	uint64_t mmapSize;
	uint64_t mmapDescSize;
<<<<<<< HEAD
	rsdp2_t *rsdp;
=======
>>>>>>> 2b1eebe50b87486f84251295194968295669c374
} boot_info_t;

extern uint64_t _kernelstart;
extern uint64_t _kernelend;

typedef struct kernel_info
{
    PageTableManager *ptm;
} kernel_info_t;

kernel_info_t kernel_init(boot_info_t *bi);

#endif