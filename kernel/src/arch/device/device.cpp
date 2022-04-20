#include <zetaos/video/tty.h>
#include <zetaos/io/IO.h>
#include <zetaos/mm/heap.h>
#include <zetaos/device/device.h>
#include <zetaos/zetalib.h>
#include <zetaos/rtc/rtc.h>

device_t *devices = 0;
uint8_t lastid = 0;

void device_init()
{
	devices = (device_t*)malloc(64*sizeof(device_t));
	memset(devices, 0, 64*sizeof(device_t));
	lastid = 0;

}

void device_print_out()
{
	for(int i = 0; i < lastid; i++)
	{
		//if(!devices[lastid]) return;
		Print(0xFFFFFF, "\nid: ");
        Print(0xFFFFFF, to_string64((uint64_t)i));
        Print(0xFFFFFF, ", unique: ");
        Print(0xFFFFFF, to_string64((uint64_t)devices[i].unique_id));
        Print(0xFFFFFF, ", ");
        Print(0xFFFFFF, devices[i].name);
	}
}

int device_add(device_t* dev)
{
	devices[lastid] = *dev;
	lastid++;
	return lastid-1;
}

device_t *device_get_by_id(uint32_t id)
{
	for(int i = 0;i < 64; i++)
	{
		if(devices[i].unique_id == id) return &devices[i];
	}
	return 0;
}

int device_getnumber()
{
	return lastid;
}

device_t *device_get(uint32_t id)
{
	return &devices[id];
}