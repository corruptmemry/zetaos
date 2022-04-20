#include <zetaos/zetalib.h>
#include <zetaos/video/tty.h>
#include <zetaos/mm/heap.h>
#include <zetaos/serial/serial.h>

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t memory_used = 0;


void *kerneld;
BootInfo* bootInfo;
void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
void mm_init(uint32_t kernel_end, BootInfo* bootInfo)
{
	last_alloc = kernel_end + 0x1000;
	heap_begin = last_alloc;
	malloc(128);
    Print(0xFFFFFF, "\nlast_alloc: 0x");
	Print(0xFFFFFF, (char*)to_hstring64((uint64_t)last_alloc));
	malloc(1);
    Print(0xFFFFFF, "\nlast_alloc: 0x");
	Print(0xFFFFFF, (char*)to_hstring64((uint64_t)last_alloc));
	Print(0xFFFFFF, "\nMemory : ");
	Print(0xFFFFFF, to_string64(((uint64_t)memory_used / 1000) / 1000));
	Print(0xFFFFFF, "/");
	Print(0xFFFFFF, to_string64((mem_size / 1000) / 1000 ));
	Print(0xFFFFFF, " used");
}
void mm_printout(){
	Print(0xFFFFFF, "\nMemory : ");
	Print(0xFFFFFF, to_string64(((uint64_t)memory_used / 1000) / 1000));
	Print(0xFFFFFF, "/");
	Print(0xFFFFFF, to_string64((mem_size / 1000) / 1000 ));
	Print(0xFFFFFF, " used");
}
void free(void *mem)
{
	alloc_t *alloc = (alloc_t*)mem - sizeof(alloc_t);
	memory_used -= alloc->size + sizeof(alloc_t);
	alloc->status = 0;
}

char* malloc(size_t size)
{
	if(!size) return 0;
	uint8_t *mem = (uint8_t *)heap_begin;
	while((uint32_t)mem < last_alloc)
	{
		alloc_t *a = (alloc_t *)mem;
		if(!a->size)
			goto nalloc;
		if(a->status) {
			mem += a->size;
			mem += sizeof(alloc_t);
			mem += 4;
			continue;
		}
		if(a->size >= size)
		{
			a->status = 1;

			memset(mem + sizeof(alloc_t), 0, size);
			memory_used += size + sizeof(alloc_t);
			return (char *)(mem + sizeof(alloc_t));
		}
		mem += a->size;
		mem += sizeof(alloc_t);
		mem += 4;
	}


	nalloc:;
	alloc_t *alloc = (alloc_t *)last_alloc;
	alloc->status = 1;
	alloc->size = size;

	last_alloc += size;
	last_alloc += sizeof(alloc_t);
	last_alloc += 4;
	memory_used += size + 4 + sizeof(alloc_t);
	memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
	return (char *)((uint32_t)alloc + sizeof(alloc_t));
}