#ifndef __ACPI_H__
#define __ACPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct rsdp2
{
    uint8_t sig[8];
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t rev;
    uint32_t rsdt_addr;
    uint32_t len;
    uint64_t xsdt_addr;
    uint8_t checksum_ext;
    uint8_t reserved[3];
} __attribute__((packed)) rsdp2_t;

typedef struct sdt_hdr
{
    uint8_t sig[4];
    uint32_t len;
    uint8_t rev;
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t oem_table_id[8];
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;
} __attribute__((packed)) sdt_header_t;

typedef struct mcfg_hdr
{
    sdt_header_t hdr;
    uint64_t reserved;
} __attribute__((packed)) mcfg_header_t;

typedef struct devcfg
{
    uint64_t base_addr;
    uint16_t pci_seg_root;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t reserved;
} __attribute__((packed)) device_config_t;

void *acpi_find_table(sdt_header_t *sdt, char *sig);

#ifdef __cplusplus
}
#endif

#endif