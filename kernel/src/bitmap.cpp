#include <bitmap.hpp>

bool Bitmap::operator[](uint64_t i)
{
    uint64_t byte_ind = i / 8;
    uint8_t bit_ind = i % 8;
    uint8_t bit_indr = 0b10000000 >> bit_ind; // bit indexer
    return (buf[byte_ind] & bit_indr) > 0;
}

void Bitmap::set(uint64_t i, bool val)
{
    uint64_t byte_ind = i / 8;
    uint8_t bit_ind = i % 8;
    uint8_t bit_indr = 0b10000000 >> bit_ind; // bit indexer
    buf[byte_ind] &= ~bit_indr;
    if (val)
        buf[byte_ind] |= bit_indr;
}