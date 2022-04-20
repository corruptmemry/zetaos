#include <zetaos/device/device.h>

#define SERIAL_PORT_A 0x3F8
#define SERIAL_PORT_B 0x2F8
extern void serial_install();
extern int serial_rcvd(int device);
extern char serial_recv(int device);
extern char serial_recv_async(int device);
extern void serial_send(int device, char out);
extern void serial_string(int device, char * out);