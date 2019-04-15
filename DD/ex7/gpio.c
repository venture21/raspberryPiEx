#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "myioctl.h"

#define GPIO_DEVICE	"gpioled"


void signal_handler(int signum)
{
	static int count;
	printf("user app: signal is catched\n");
	if(signum==SIGIO)
	{
		printf("count:%d\n",count);
	}
	count++;
}


int main(int argc, char** argv)
{
	char buf[BUFSIZ];
	char devPath[50];
	int i=0;
	int fd;
	int count;
	int data;
	unsigned long temp;
	memset(buf, 0, BUFSIZ);
	
	signal(SIGIO, signal_handler);

	sprintf(devPath, "/dev/%s", GPIO_DEVICE);
	printf("argv[1]:%s\n", argv[1]);
	chmod(devPath, 0666);
	fd=open(devPath,O_RDWR);
	if(fd<0)
	{
		printf("Error open()\n");
		return -1;	
	}

	sprintf(buf,"%s:%d",argv[1],getpid());
	count=write(fd, buf, strlen(buf));
	if(count<0)
	{
		printf("Error write()\n");
	}

	if (ioctl(fd, CMD1, 10) < 0)
		printf("Error:ioctl1\n");

	if (ioctl(fd, CMD2, 21)<0)
		printf("Error:ioctl2\n");

	if (ioctl(fd, CMD3, 32)<0)
		printf("Error:ioctl3\n");

	close(fd);
	return 0;

}
