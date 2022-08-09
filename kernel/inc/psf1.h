#ifndef __PSF1_H__
#define __PSF1_H__

typedef struct psf1hdr
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t chsize;
} psf1hdr_t;

typedef struct psf1fnt
{
    PSF1Header *psf1hdr;
    void *glyphs;
} psf1fnt_t;

#endif