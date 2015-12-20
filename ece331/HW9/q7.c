/*#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
//#include <linux/in.h>
#include <string.h>

int main(int argc, char *argv[])
{
	struct ifreq eth;
	char string[4] = {'e','t','h','0'};
	int socketfd = 0;
	int i = 0;
	int ret = 0;
	for(i = 0; i < 4; i++){
		eth.ifr_name[i] = string[i];
	}
	if((socketfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_IP) == -1 )){
		perror("error1: ");
		close(socketfd);
		return -1;
	}
	if((ret = ioctl(socketfd, SIOCGIFHWADDR, &eth)) < 0){
		perror("error2: ");
		close(socketfd);
		return -1;
	}
	printf("%02X:%02X:%02X:%02X:%02X:%02X\n",eth.ifr_hwaddr.sa_data[0]),
					    	 eth.ifr_hwaddr.sa_data[1]),
					    	 eth.ifr_hwaddr.sa_data[2]),
					    	 eth.ifr_hwaddr.sa_data[3]),
					         eth.ifr_hwaddr.sa_data[4]),
					    	 eth.ifr_hwaddr.sa_data[5]));
	close(socketfd);
	return 0;
}*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>

int main(int argc, char* argv[])
{
	struct ifreq eth;
	char string[4] = {'e','t','h','0'};
	int socketfd = 0;
	int i = 0;
	
	for(i = 0; i < 4; i++){
		eth.ifr_name[i] = string[i]; //set ifr_name to eth0
	}

	if ((socketfd = socket(AF_INET,SOCK_DGRAM, IPPROTO_IP)) == -1) { //create a socket and a socket file descriptor
		perror("error");
		close(socketfd);
		return -1;
	}
	
	if (ioctl(socketfd, SIOCGIFHWADDR, &eth) < 0) { //get the MAC address info
		perror("error");
		close(socketfd);
		return -1;
	}

	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",eth.ifr_hwaddr.sa_data[0], //print to the screen
						 eth.ifr_hwaddr.sa_data[1],
						 eth.ifr_hwaddr.sa_data[2],
						 eth.ifr_hwaddr.sa_data[3],
						 eth.ifr_hwaddr.sa_data[4],
						 eth.ifr_hwaddr.sa_data[5]);
 
	close(socketfd);
	return 0;
}
