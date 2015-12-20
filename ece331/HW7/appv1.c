#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include "v1.h"
#define DEVICE "/dev/ECE_331_Device"

int main(int argc, char * argv[])
{
	int fd;
	char ch;
	uint64_t u;
	virtual_device v;


	fd = open(DEVICE, O_RDWR);
	if(fd == -1){
		printf("error: file %s does not exist, is locked by another process or you do not have permission to use this device\n",DEVICE);
	exit(-1);
	}
	printf("r=read\nw=write\n");
	scanf ("%c", &ch);
	
	switch (ch) {
		case 'w':
			printf("enter data: ");
			scanf(" %lld", &u);
			printf("data written to device was %lld",u);
			v.data = u;
			if(ioctl(fd, WRITE_DEVICE, &v) == -1){
				perror("failed");
			}
			break;

		case 'r':
			if (ioctl(fd, READ_DEVICE, &v) == -1){
				perror("failed");
			}
			printf("device: data = %lld\n", v.data);
			break;

		default:
			printf("command not recongnized\n");
			break;
	}
	close(fd);
	return 0;
}
