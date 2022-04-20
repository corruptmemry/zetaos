#include <zetaos/mm/heap.h>
#include <zetaos/device/device.h>
#include <zetaos/video/tty.h>
#include <zetaos/zetalib.h>
#include <zetaos/rng.h>
int random_id = 0;

void random_read(uint8_t* buffer, uint32_t offset, uint32_t len, device_t *dev)
{
    asm volatile(
        "mov %%ecx, 100 \n\t\
        .retry: \n\t\
        rdseed %%eax \n\t\
        jc .done \n\t\
        loop .retry \n\t\
        .done: " : "=r"(buffer));
    return buffer;
}

void generate_onetimeseed()
{
	uint8_t* buffer = (char*)malloc(32);
	device_t *random = device_get(random_id);
	Print(0xFFFAAA, "\nrandom : generated a one-time seed: ");
	Print(0xFFFAAA, to_string64(random->read(buffer, 0, 32, random) * random->read(buffer, 0, 32, random) * random->read(buffer, 0, 32, random)));
}

void create_rng()
{
	device_t *random = 0;
	random = (device_t*)malloc(sizeof(device_t));
	random->name = "/dev/random";
	random->unique_id = 0x1010;
	random->dev_type = DEVICE_CHAR;
	random->read = (uint8_t(*)(uint8_t*, uint32_t, uint32_t, device_t*))random_read;
	random = device_add(random);
	generate_onetimeseed();
}
