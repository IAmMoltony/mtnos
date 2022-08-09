#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>
#include <stdbool.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct framebuffer
{
	void *base_addr;
	size_t size;
	uint32_t width;
	uint32_t height;
	uint32_t px_per_scanline;
} Framebuffer;

typedef struct psf1hdr
{
	uint8_t magic[2];
	uint8_t mode;
	uint8_t chsize;
} PSF1Header;

typedef struct psf1fnt
{
	PSF1Header *psf1hdr;
	void *glyphs;
} PSF1Font;

EFI_FILE *load_file(EFI_FILE *dir, CHAR16 *path, EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable)
{
	EFI_FILE *ldFile;
	EFI_LOADED_IMAGE_PROTOCOL *ldImg;
	sysTable->BootServices->HandleProtocol(imgHandle, &gEfiLoadedImageProtocolGuid, (void **)&ldImg);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
	sysTable->BootServices->HandleProtocol(ldImg->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid,
										   (void **)&fs);

	if (dir == NULL)
		fs->OpenVolume(fs, &dir);

	EFI_STATUS st = dir->Open(dir, &ldFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (st != EFI_SUCCESS)
		return NULL;
	return ldFile;
}

int memcmp(const void *ap, const void *bp, size_t n)
{
	const uint8_t *a = ap, *b = bp;
	for (size_t i = 0; i < n; ++i)
	{
		if (a[i] < b[i])
			return -1;
		else if (a[i] > b[i])
			return 1;
	}
	return 0;
}

Framebuffer fb;
bool gop_init(void)
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_STATUS st;

	st = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void **)&gop);
	if (EFI_ERROR(st))
	{
		Print(L"Unable to locate GOP\r\n");
		return false;
	}
	Print(L"GOP located\r\n");
	fb.base_addr = (void *)gop->Mode->FrameBufferBase;
	fb.size = gop->Mode->FrameBufferSize;
	fb.width = gop->Mode->Info->HorizontalResolution;
	fb.height = gop->Mode->Info->VerticalResolution;
	fb.px_per_scanline = gop->Mode->Info->PixelsPerScanLine;

	return true;
}

PSF1Font *font;
bool load_font(EFI_FILE *dir, CHAR16 *path, EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable)
{
	EFI_FILE *ffile = load_file(dir, path, imgHandle, sysTable); // font file
	if (ffile == NULL)
	{
		Print(L"Could not load font %s", path);
		return false;
	}

	PSF1Header *fhdr; // font header
	sysTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1Header), (void **)&fhdr);
	UINTN size = sizeof(PSF1Header);
	ffile->Read(ffile, &size, fhdr);

	if (fhdr->magic[0] != PSF1_MAGIC0 || fhdr->magic[1] != PSF1_MAGIC1)
	{
		Print(L"Incorrect font format\r\n");
		return false;
	}

	UINTN glyphbufSize = fhdr->chsize * 256;
	if (fhdr->mode == 1)
		glyphbufSize = fhdr->chsize * 512;

	void *glyphs;
	{
		ffile->SetPosition(ffile, sizeof(PSF1Header));
		sysTable->BootServices->AllocatePool(EfiLoaderData, glyphbufSize, (void **)&glyphs);
		ffile->Read(ffile, &glyphbufSize, glyphs);
	}

	sysTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1Font), (void **)&font);
	font->psf1hdr = fhdr;
	font->glyphs = glyphs;

	return true;
}

EFI_STATUS efi_main(EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable)
{
	InitializeLib(imgHandle, sysTable);
	Print(L"MtnOS is booting\r\n");
	uefi_call_wrapper(ST->ConOut->EnableCursor, 2, ST->ConOut, FALSE);

	EFI_FILE *kernel = load_file(NULL, L"kernel.elf", imgHandle, sysTable);
	if (!kernel)
	{
		Print(L"Could not load kernel.elf\r\n");
		return EFI_LOAD_ERROR;
	}
	Print(L"Kernel loaded\r\n");

	Elf64_Ehdr hdr;
	{
		UINTN fiSize; // file info size
		EFI_FILE_INFO *fi;
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fiSize, NULL);
		sysTable->BootServices->AllocatePool(EfiLoaderData, fiSize, (void **)&fi);
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fiSize, (void **)&fi);

		UINTN size = sizeof(hdr);
		kernel->Read(kernel, &size, &hdr);
	}

	if (
		memcmp(&hdr.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		hdr.e_ident[EI_CLASS] != ELFCLASS64 ||
		hdr.e_ident[EI_DATA] != ELFDATA2LSB ||
		hdr.e_type != ET_EXEC ||
		hdr.e_machine != EM_X86_64 ||
		hdr.e_version != EV_CURRENT)
	{
		Print(L"Bad kernel\r\n");
		return EFI_UNSUPPORTED;
	}
	Print(L"Kernel format ok\r\n");

	Elf64_Phdr *phdrs;
	{
		kernel->SetPosition(kernel, hdr.e_phoff);
		UINTN size = hdr.e_phnum * hdr.e_phentsize;
		sysTable->BootServices->AllocatePool(EfiLoaderData, size, (void **)&phdrs);
		kernel->Read(kernel, &size, phdrs);
	}

	for (Elf64_Phdr *phdr = phdrs;
		 (char *)phdr < (char *)phdrs + hdr.e_phnum * hdr.e_phentsize;
		 phdr = (Elf64_Phdr *)((char *)phdr + hdr.e_phentsize))
	{
		if (phdr->p_type == PT_LOAD)
		{
			int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
			Elf64_Addr seg = phdr->p_paddr;
			sysTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &seg);

			kernel->SetPosition(kernel, phdr->p_offset);
			UINTN size = phdr->p_filesz;
			kernel->Read(kernel, &size, (void *)seg);
		}
	}
	Print(L"Successfully loaded kernel\r\n");

	if (!gop_init())
		return EFI_UNSUPPORTED;
	Print(L"base_addr: 0x%x\r\nsize: 0x%x\r\nwidth: %d\r\nheight: %d\r\npx_per_scanline: %d\r\n",
		  fb.base_addr, fb.size, fb.width, fb.height, fb.px_per_scanline);

	if (!load_font(NULL, L"zap-vga16.psf", imgHandle, sysTable))
		return EFI_LOAD_ERROR;
	Print(L"Loaded font, chsize: %d\r\n", font->psf1hdr->chsize);

	void (*kstart)(Framebuffer *, PSF1Font *) = ((__attribute__((sysv_abi)) void (*)(Framebuffer *, PSF1Font *))hdr.e_entry);
	kstart(&fb, font);

	return EFI_SUCCESS;
}