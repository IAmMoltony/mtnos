#include <acpi.h>
#include <stddef.h>

void *acpi_find_table(sdt_header_t *sdt, char *sig)
{
    int entries = (sdt->len - sizeof(sdt_header_t)) / 8;
    for (int i = 0; i < entries; ++i)
    {
        // casting address of xsdt to uint64 so we can do some math on it,
        // add size so we get the end and adding i * 8 to get new address
        // and turning that into sdt_header_t *
        sdt_header_t *new_hdr = (sdt_header_t *)*(uint64_t *)((uint64_t)sdt + sizeof(sdt_header_t) + (i * 8));
        for (int j = 0; j < 4; ++j)
        {
            if (new_hdr->sig[j] != sig[j])
                break;
            
            if (j == 3)
                return new_hdr;
        }
    }
    return NULL;
}