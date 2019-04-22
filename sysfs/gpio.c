/*
$ echo "18" > /sys/class/gpio/export
$ echo "out" > /sys/class/gpio/gpio18/direction
$ echo "1" > /sys/class/gpio/gpio18/value
$ echo "18" > /sys/class/gpio/unexport
*/

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
#define GPIO_IN		0
#define GPIO_OUT	1

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

int gpioUnexport(int gpionum)
{
	int fd, length;
	char cmdBuf[MAX_BUF];

	fd = open(SYS_GPIO_DIR "unexport", O_WRONLY);
	if (fd < 0)
	{
		char errBuf[128];
		sprintf(errBuf, "gpioUnexport unable to open : gpio%d", gpionum);
		perror(errBuf);
		return fd;
	}
	sprintf(cmdBuf, "%d", gpionum);
	if (write(fd, cmdBuf, strlen(cmdBuf)) < 0)
	{
		perror("Error:gpioUnexport");
		return fd;
	}
	close(fd);
}

int gpioDirection(int gpionum, int dir)
{
	int fd, length;
	char cmdBuf[MAX_BUF];

	sprintf(cmdBuf, "%s/gpio%d/direction", SYS_GPIO_DIR, gpionum);
	fd = open(cmdBuf, O_WRONLY);
	if (fd < 0)
	{
		char errBuf[128];
		sprintf(errBuf, "gpioDirection unable to open : gpio%d", gpionum);
		perror(errBuf);
		return fd;
	}
	if(dir)	
		sprintf(cmdBuf, "out");
	else 
		sprintf(cmdBuf, "in");

	if (write(fd, cmdBuf, strlen(cmdBuf)) < 0)
	{
		perror("Error:gpioDirection");
		return fd;
	}
	close(fd);
}


int gpioValue(int gpionum, int value)
{
	int fd, length;
	char cmdBuf[MAX_BUF];

	sprintf(cmdBuf, "%s/gpio%d/value", SYS_GPIO_DIR, gpionum);
	fd = open(cmdBuf, O_WRONLY);
	if (fd < 0)
	{
		char errBuf[128];
		sprintf(errBuf, "gpioDirection unable to open : gpio%d", gpionum);
		perror(errBuf);
		return fd;
	}

	sprintf(cmdBuf, "%d", value);
	
	if (write(fd, cmdBuf, strlen(cmdBuf)) < 0)
	{
		perror("Error:gpioValue");
		return fd;
	}
	close(fd);
}


int main(void)
{
	gpioExport(LED);
	gpioDirection(LED, GPIO_OUT);
	gpioValue(LED, 1);
	gpioUnexport(LED);
}