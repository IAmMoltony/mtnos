#ifndef __BITMAP_HPP__
#define __BITMAP_HPP__

#include <stddef.h>
#include <stdint.h>

class Bitmap
{
public:
    size_t size;
    uint8_t *buf;
    bool operator[](uint64_t i);
    bool set(uint64_t i, bool val);
};

#endif