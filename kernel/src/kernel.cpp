#include <zetaos/zetalib.h>
#include <zetaos/video/tty.h>
#include <zetaos/io/IO.h>
#include <zetaos/mm/heap.h>
#include <zetaos/serial/serial.h>
#include <zetaos/gdt/gdt.h>
#include <cstdarg>
#include <zetaos/drivers/ata/ata.h>
#include <zetaos/pci/pci.h>
#include <zetaos/idt/idt.h>
#include <zetaos/rtc/rtc.h>
#include <zetaos/device/device.h>
#include <zetaos/sched/pit.h>
#include <zetaos/rng.h>

int dummy_id = 0;
int mem_size = -1;

uint64_t get_mem_size(EFI_MEMORY_DESCRIPTOR* MemoryMapFirstDescriptor, uint64_t MemoryMapEntries, uint64_t MemoryMapDescriptorSize){

    static uint64_t MemorySizeBytes = 0;
    if (MemorySizeBytes > 0) return MemorySizeBytes;

    for (int i = 0; i < MemoryMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMapFirstDescriptor + (i * MemoryMapDescriptorSize));
        MemorySizeBytes += Descriptor->numPages * 4096;
    }

    return MemorySizeBytes;

}

extern "C" void _start(BootInfo* bootInfo){
    init_video(bootInfo->framebuffer, bootInfo->psf1_Font);
    mm_init(bootInfo->kernelEnd, bootInfo);
    device_init();
    serial_install();
    Print(0xFFFFFF, "\nINITIALIZE KERNEL");
    init_idt();
    mem_size = get_mem_size(bootInfo->mMap, bootInfo->mMapSize / bootInfo->mMapDescSize, bootInfo->mMapDescSize);

    cpu_detect();

    Print(0xFFFFFF, "\n_start : Probing PCI hardware...");
    pci_init();

    Print(0xFFFFFF, "\n_start : Creating CHAR devices...");
    create_rng();
    
    device_print_out();
    mm_printout();
    return ;
}