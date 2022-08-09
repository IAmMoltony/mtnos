OSNAME = CustomOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = linker.ld
CC = gcc

CFLAGS = -ffreestanding -fshort-wchar -Wall -Wextra -Iinc -c
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(call rwildcard,$(SRCDIR),*.c)
CPPSRC = $(call rwildcard,$(SRCDIR),*.cpp)
COBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CSRC))
CPPOBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(CPPSRC))
DIRS = $(wildcard $(SRCDIR)/*)

$(BUILDDIR)/kernel.elf: $(COBJS) $(CPPOBJS)
	@echo linking $(notdir $@)
	@$(LD) $(LDFLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo $(notdir $^)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo $(notdir $^)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/$(OSNAME).img: $(BUILDDIR)/kernel.elf $(BOOTEFI) startup.nsh
	@echo building $@...
	@dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	@mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 ::
	@mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	@mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	@mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-vga16.psf ::

image: $(BUILDDIR)/$(OSNAME).img

clean:
	@rm -rf $(OBJDIR)/*.o
	@rm -rf $(BUILDDIR)/*.img
	@rm -rf $(BUILDDIR)/*.elf

run:
	@echo run...
	@qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none

.PHONY: run image clean
