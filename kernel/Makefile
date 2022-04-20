
OSNAME = Zeta

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld
CC = g++
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -Isrc/include/ -g -w -mgeneral-regs-only -mno-red-zone -fno-exceptions -fpermissive
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib 

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)          
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.o, $(ASMSRC))
OBJS += lib/arch/gdt/gdt_asm.o
DIRS = $(wildcard $(SRCDIR)/*)
ASMC = nasm
ASMFLAGS = 

kernel: $(OBJS) link

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo "  CC     $^"
	@ mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@ > /dev/null
	

link:
	@echo "  LD    $(OBJS)"
	@$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS) > /dev/null

clean:
	rm lib/*/*.o

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg:
	@ echo "  DD     $(OSNAME).img"
	@dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750 &> /dev/null
	@mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 :: > /dev/null
	@mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI > /dev/null
	@mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT > /dev/null
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT > /dev/null
	@mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh :: > /dev/null
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf :: > /dev/null
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-light16.psf :: > /dev/null
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/exboot.conf :: > /dev/null

run:
	@qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -serial file:/dev/stdout -s -m 64M -cpu EPYC-v3 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
