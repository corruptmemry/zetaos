#include <zetaos/zetalib.h>

extern void paging_init();
extern void paging_enable();
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);