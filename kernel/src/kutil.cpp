#include <kutil.hpp>
#include <gdt/gdt.h>
#include <interrupts/idt.hpp>
#include <interrupts/interrupts.hpp>
#include <basicrend.hpp>

static kernel_info_t ki;
static PageTableManager ptm = NULL;

void prepare_mem(boot_info_t *bi)
{
    uint64_t mmap_entries = bi->mmapSize / bi->mmapDescSize;
    g_pfalloc = PageFrameAlloc();
    g_pfalloc.read_efi_mmap(bi->mmap, bi->mmapSize, bi->mmapDescSize);

    uint64_t kernel_size = (uint64_t)&_kernelend - (uint64_t)&_kernelstart;
    uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;
    g_pfalloc.lock_pages(&_kernelstart, kernel_pages);

    page_table_t *pml4 = (page_table_t *)g_pfalloc.req_page();
    memset(pml4, 0, 0x1000);
    ptm = PageTableManager(pml4);

    for (uint64_t i = 0; i < get_memsize(bi->mmap, mmap_entries, bi->mmapDescSize); i += 0x1000)
    {
        ptm.map_mem((void *)i, (void *)i);
    }
    uint64_t fb_base = (uint64_t)bi->fb->base_addr;
    uint64_t fb_size = (uint64_t)bi->fb->size + 0x1000;
    g_pfalloc.lock_pages((void *)fb_base, fb_size / 0x1000 + 1);
    for (uint64_t i = fb_base; i < fb_base +fb_size; i += 0x1000)
    {
        ptm.map_mem((void *)i, (void *)i);
    }

    asm("mov %0, %%cr3" : : "r"(pml4));
    // clear framebuffer
    memset(bi->fb->base_addr, 0, bi->fb->size);
    ki.ptm = &ptm;
}

IDTR idtr;
void prepare_interrupts(void)
{
    idtr.limit = 0x0fff;
    idtr.off = (uint64_t)g_pfalloc.req_page();

    // page fault
    IDTDescEntry *int_page_fault = (IDTDescEntry *)(idtr.off + 0xe * sizeof(IDTDescEntry));
    int_page_fault->set_off((uint64_t)page_fault_handler);
    int_page_fault->type_attr = IDT_TYPE_ATTR_INTERRUPT_GATE;
    int_page_fault->selector = 0x08;
    
    // double fault
    IDTDescEntry *int_double_fault = (IDTDescEntry *)(idtr.off + 0x8 * sizeof(IDTDescEntry));
    int_double_fault->set_off((uint64_t)double_fault_handler);
    int_double_fault->type_attr = IDT_TYPE_ATTR_INTERRUPT_GATE;
    int_double_fault->selector = 0x08;

    // gp fault
    IDTDescEntry *int_gp_fault = (IDTDescEntry *)(idtr.off + 0xd * sizeof(IDTDescEntry));
    int_gp_fault->set_off((uint64_t)gp_fault_handler);
    int_gp_fault->type_attr = IDT_TYPE_ATTR_INTERRUPT_GATE;
    int_gp_fault->selector = 0x08;

    asm("lidt %0" : : "m"(idtr));
}

BasicRenderer rend = BasicRenderer(NULL, NULL);
kernel_info_t kernel_init(boot_info_t *bi)
{
    rend = BasicRenderer(bi->fb, bi->font);
    g_renderer = &rend;

    gdt_desc_t gdtdesc;
    gdtdesc.size = sizeof(gdt_t) - 1;
    gdtdesc.off = (uint64_t)&gdt_default;
    load_gdt(&gdtdesc);
    prepare_mem(bi);
    prepare_interrupts();
    return ki;
}