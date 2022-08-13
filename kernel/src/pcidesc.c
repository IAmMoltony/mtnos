#include <stdint.h>
#include <str.h>

static char hex_out[5];

const char *pci_dev_classes[] = {
    "unclassified",
    "mass storage controller",
    "network controller",
    "display controller",
    "multimedia controller",
    "memory controller",
    "bridge device",
    "simple communication controller",
    "base system peripheral",
    "input device controller",
    "docking station",
    "processor",
    "serial bus controller",
    "wireless controller",
    "intelligent controller",
    "satellite communication controller",
    "encryption controller",
    "signal processing controller",
    "processing accelerator",
    "non essential instrumentation",
};

const char *get_vendor_name(uint16_t vendor_id)
{
    switch (vendor_id)
    {
    case 0x8086:
        return "Intel Corp";
    case 0x1022:
        return "AMD";
    case 0x10de:
        return "NVIDIA Corporation";
    }

    longtostr(vendor_id, hex_out, 16);
    return hex_out;
}


const char *get_dev_name(uint16_t vendor_id, uint16_t dev_id)
{
    switch (vendor_id)
    {
    case 0x8086: // intel
        switch (dev_id)
        {
        case 0x29c0:
            return "Express DRAM controller";
        case 0x2918:
            return "LPC interface controller";
        case 0x2922:
            return "6 port SATA controller [AHCI mode]";
        case 0x2930:
            return "SMBus controller";
        }
    }

    longtostr(dev_id, hex_out, 16);
    return hex_out;
}

const char *mass_storage_con_subclass_name(uint8_t subclass_code)
{
    switch (subclass_code)
    {
    case 0x00:
        return "SCSI bus controller";
    case 0x01:
        return "IDE controller";
    case 0x02:
        return "Floppy disk controller";
    case 0x03:
        return "IPI bus controller";
    case 0x04:
        return "RAID controller";
    case 0x05:
        return "ATA controller";
    case 0x06:
        return "Serial ATA";
    case 0x07:
        return "Serial attached SCSI";
    case 0x08:
        return "Non-volatile memory controller";
    case 0x80:
        return "other";
    }
    
    longtostr(subclass_code, hex_out, 16);
    return hex_out;
}

const char *get_subclass_name(uint8_t class_code, uint8_t subclass_code)
{
    switch (class_code)
    {
    case 0x01:
        return mass_storage_con_subclass_name(subclass_code);
    }

    longtostr(subclass_code, hex_out, 16);
    return hex_out;
}

const char *get_prog_interface_name(uint8_t class_code, uint8_t subclass_code, uint8_t prog_interface)
{
    switch (class_code)
    {
    case 0x01: // mass storage con
        switch (subclass_code)
        {
        case 0x06: // serial ata
            switch (prog_interface)
            {
            case 0x00:
                return "Vendor specific interface";
            case 0x01:
                return "AHCI 1.0";
            case 0x02:
                return "Serial storage bus";
            }
        }
    }

    longtostr(prog_interface, hex_out, 16);
    return hex_out;
}