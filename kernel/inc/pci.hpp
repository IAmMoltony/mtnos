#ifndef __PCI_H__
#define __PCI_H__

#include <acpi.h>
#include <stdint.h>

typedef struct pci_devhdr
{
    uint16_t vendor_id;
    uint16_t dev_id;
    uint16_t cmd;
    uint16_t status;
    uint8_t rev_id;
    uint8_t prog_interface;
    uint8_t subclass;
    uint8_t class_; // theres an underscore because class keyword in c++
    uint8_t cache_line_size;
    uint8_t latency_tmr;
    uint8_t hdr_type;
    uint8_t bist;
} pci_device_hdr_t;

void pci_enumerate(mcfg_header_t *mcfg);

extern "C" const char *pci_dev_classes[];
extern "C" const char *get_vendor_name(uint16_t vendor_id);
extern "C" const char *get_dev_name(uint16_t vendor_id, uint16_t dev_id);
extern "C" const char *get_subclass_name(uint8_t class_code, uint8_t subclass_code);

#endif