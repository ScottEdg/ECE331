#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/ECE_331_Device"

int main(int argc, char * argv[])
{
//	unsigned long long int test, write_val, read_val;
//	int k = sizeof(test);
//	printf("%d\n",k);
	int fd;
	char ch, write_data[100];

struct fake_device{
	char data[100];
//	struct semeaphore sem;
}virtual_device;

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
			scanf(" %[^\n]", write_data);
			//write_val = atoll(write_data);
		//	printf("data written to device was %s",write_data);
			write(fd, write_data, sizeof(write_data));
			break;

		case 'r':
			//read(fd, read_data, sizeof(read_data));
			//read_val = atoll(read_data);
			
			if (ioctl(fd, _IOR('v', 1, struct fake_device *), &virtual_device) == -1){
				perror("failed");
			}
			printf("device: %s\n", virtual_device.data);
			break;

		default:
			printf("command not recongnized\n");
			break;
	}
	close(fd);
	return 0;
}
