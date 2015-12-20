#include <inttypes.h>
#include <linux/ioctl.h>

typedef struct{
	uint64_t data;
} virtual_device;

#define READ_DEVICE _IOR('v', 1, virtual_device *)
#define WRITE_DEVICE _IOW('v', 2, virtual_device *)
