#include <kutil.hpp>
#include <gdt/gdt.h>
#include <interrupts/idt.hpp>
#include <interrupts/interrupts.hpp>
#include <basicrend.hpp>
#include <io.h>
#include <input/mouse.hpp>
#include <acpi.h>
#include <pci.hpp>

static kernel_info_t ki;

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
    g_ptm = PageTableManager(pml4);

    for (uint64_t i = 0; i < get_memsize(bi->mmap, mmap_entries, bi->mmapDescSize); i += 0x1000)
    {
        g_ptm.map_mem((void *)i, (void *)i);
    }
    uint64_t fb_base = (uint64_t)bi->fb->base_addr;
    uint64_t fb_size = (uint64_t)bi->fb->size + 0x1000;
    g_pfalloc.lock_pages((void *)fb_base, fb_size / 0x1000 + 1);
    for (uint64_t i = fb_base; i < fb_base +fb_size; i += 0x1000)
    {
        g_ptm.map_mem((void *)i, (void *)i);
    }

    asm("mov %0, %%cr3" : : "r"(pml4));
    // clear framebuffer
    memset(bi->fb->base_addr, 0, bi->fb->size);
    ki.ptm = &g_ptm;
}

IDTR idtr;

void set_idt_gate(void *handler, uint8_t off, uint8_t type_attr, uint8_t selector)
{
    IDTDescEntry *interrupt = (IDTDescEntry *)(idtr.off + off * sizeof(IDTDescEntry));
    interrupt->set_off((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

void prepare_interrupts(void)
{
    idtr.limit = 0x0fff;
    idtr.off = (uint64_t)g_pfalloc.req_page();

    // page fault
    set_idt_gate((void *)page_fault_handler, 0xe, IDT_TYPE_ATTR_INTERRUPT_GATE, 0x08);
    
    // double fault
    set_idt_gate((void *)double_fault_handler, 0x8, IDT_TYPE_ATTR_INTERRUPT_GATE, 0x08);

    // gp fault
    set_idt_gate((void *)gp_fault_handler, 0xd, IDT_TYPE_ATTR_INTERRUPT_GATE, 0x08);

    // keyboard
    set_idt_gate((void *)keyboard_int_handler, 0x21, IDT_TYPE_ATTR_INTERRUPT_GATE, 0x08);

    // mouse
    set_idt_gate((void *)mouse_int_handler, 0x2c, IDT_TYPE_ATTR_INTERRUPT_GATE, 0x08);

    asm("lidt %0" : : "m"(idtr));
    remap_pic();
    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);
}

void prepare_acpi(boot_info_t *bi)
{
    sdt_header_t *xsdt = (sdt_header_t *)(bi->rsdp->xsdt_addr);
    mcfg_header_t *mcfg = (mcfg_header_t *)acpi_find_table(xsdt, (char *)"MCFG");
    
    pci_enumerate(mcfg);
}

BasicRenderer rend = BasicRenderer(NULL, NULL);
kernel_info_t kernel_init(boot_info_t *bi)
{
    rend = BasicRenderer(bi->fb, bi->font);
    g_renderer = &rend;
    g_renderer->set_default_color(0xaaaaaaaa);
    g_renderer->set_clear_color(0);
    g_renderer->clear();

    gdt_desc_t gdtdesc;
    gdtdesc.size = sizeof(gdt_t) - 1;
    gdtdesc.off = (uint64_t)&gdt_default;
    load_gdt(&gdtdesc);

    prepare_mem(bi);
    prepare_interrupts();
    ps2mouse_init();
    prepare_acpi(bi);

    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0b11101111);

    asm("sti");

    return ki;
}