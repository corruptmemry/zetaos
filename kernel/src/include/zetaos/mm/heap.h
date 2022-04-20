
#ifndef __MEMORY_H_
#define __MEMORY_H_
#include <zetaos/zetalib.h>

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

extern void mm_init(uint32_t kernel_end, BootInfo* bootInfo);
extern void mm_print_out();

extern void paging_init();
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);

extern char* pmalloc(size_t size); /* page aligned alloc */
extern char* malloc(size_t size);
extern void free(void *mem);
extern void* memset(void* bufptr, int value, size_t size);
extern void mm_printout();
#endif