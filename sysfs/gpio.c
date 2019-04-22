#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>


#define LED		18	//GPIO18
#define MAX_BUF	100 
#define SYS_GPIO_DIR	"/sys/class/gpio/"

int gpioExport(int gpionum)
{
	int fd, length;
	char cmdBuf[MAX_BUF];

	fd = open(SYS_GPIO_DIR "export", O_WRONLY);
	if (fd < 0)
	{
		char errBuf[128];
		sprintf(errBuf, "gpioExport unable to open : gpio%d", gpionum);
		perror(errBuf);
		return fd;
	}
	sprintf(cmdBuf, "%d", gpionum);
	if (write(fd, cmdBuf, strlen(cmdBuf)) < 0)
	{
		perror("Error:gpioExport");
		return fd;
	}
	close(fd);
}


int main(void)
{
	gpioExport(LED);
	//gpioDirection(LED);
	//gpioValue(LED);
	//gpioUnexport(LED);
}