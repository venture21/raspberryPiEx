#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

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
	int i=0;
	int fd;
	int count;
	memset(buf, 0, BUFSIZ);

	signal(SIGIO, signal_handler);

	printf("argv[1]:%s\n", argv[1]);
	fd=open("/dev/gpioled",O_RDWR);
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

	//count=read(fd,buf,10);
	//printf("Read data:%s\n",buf);

	while(1);
	close(fd);
	return 0;

}
