#include <pci.hpp>
#include <paging/pagetablemgr.hpp>
#include <basicrend.hpp>

static void func_enumerate(uint64_t dev_addr, uint64_t func)
{
    // function is the last end point

    uint64_t off = func << 12;
    uint64_t func_addr = dev_addr + off;
    g_ptm.map_mem((void *)func_addr, (void *)func_addr);

    pci_device_hdr_t *pci_dev_hdr = (pci_device_hdr_t *)func_addr;
    if (pci_dev_hdr->dev_id == 0 || pci_dev_hdr->dev_id == 0xffff)
    {
        return; // not valid
    }

    g_renderer->printf("%s | %s | %s | %s | %s\n", get_vendor_name(pci_dev_hdr->vendor_id),
                       get_dev_name(pci_dev_hdr->vendor_id, pci_dev_hdr->dev_id),
                       pci_dev_classes[pci_dev_hdr->class_],
                       get_subclass_name(pci_dev_hdr->class_, pci_dev_hdr->subclass),
                       get_prog_interface_name(pci_dev_hdr->class_, pci_dev_hdr->subclass,
                       pci_dev_hdr->prog_interface));
}

static void device_enumerate(uint64_t bus_addr, uint64_t device)
{
    uint64_t off = device << 15;
    uint64_t dev_addr = bus_addr + off;
    g_ptm.map_mem((void *)dev_addr, (void *)dev_addr);

    pci_device_hdr_t *pci_dev_hdr = (pci_device_hdr_t *)dev_addr;
    if (pci_dev_hdr->dev_id == 0 || pci_dev_hdr->dev_id == 0xffff)
    {
        return; // not valid
    }
    
    // 8 functions per device
    for (uint64_t func = 0; func < 8; ++func)
    {
        func_enumerate(dev_addr, func);
    }
}

static void bus_enumerate(uint64_t base_addr, uint64_t bus)
{
    uint64_t off = bus << 20;
    uint64_t bus_addr = base_addr + off;
    g_ptm.map_mem((void *)bus_addr, (void *)bus_addr);

    pci_device_hdr_t *pci_dev_hdr = (pci_device_hdr_t *)bus_addr;
    if (pci_dev_hdr->dev_id == 0 || pci_dev_hdr->dev_id == 0xffff)
    {
        return; // not valid
    }
    
    // there are 32 devices on each bus
    for (uint64_t dev = 0; dev < 32; ++dev)
    {
        device_enumerate(bus_addr, dev);
    }
}

void pci_enumerate(mcfg_header_t *mcfg)
{
    int entries = ((mcfg->hdr.len) - sizeof(mcfg_header_t)) / sizeof(device_config_t);
    for (int i = 0; i < entries; ++i)
    {
        device_config_t *new_devcfg = (device_config_t *)((uint64_t)mcfg + sizeof(mcfg_header_t) + (sizeof(device_config_t) * i));
        for (uint64_t bus = new_devcfg->start_bus; bus < new_devcfg->end_bus; ++bus)
        {
            bus_enumerate(new_devcfg->base_addr, bus);
        }
    }
}