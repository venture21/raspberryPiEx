#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

//Raspberry Pi3 PHYSICAL I/O Peripherals BaseAddr
#define BCM_IO_BASE 0x3F000000

//GPIO_BaseAddr
#define GPIO_BASE  (BCM_IO_BASE + 0x200000)

//GPIO Function Select Register 0 ~ 5
#define GPIO_IN(g)  (*(gpio+((g)/10)) &=~(7<<(((g)%10)*3)))
#define GPIO_OUT(g)  (*(gpio+((g)/10)) |=(1<<(((g)%10)*3)))

//GPIO Pin Output Set 0 / Clr 0
//                      addr     value 
#define GPIO_SET(g)  (*(gpio+7)=(1<<g))
#define GPIO_CLR(g)  (*(gpio+10)=(1<<g))

#define GPIO_GET(g)  (*(gpio+13)&1<<g))
#define GPIO_SIZE 0xB4

volatile unsigned int *gpio;


int main(int argc, char**argv)
{
	//GPIO Pin number
	int gno;
	int i, mem_fd;
	
	void *gpio_map;
	
	//핀번호 입력이 안됐을때
	if(argc<2)
	{
		printf("Usage:%s GPIO_NO\n",argv[0]);
		return -1;
	}
	
	gno = atoi(argv[1]);
	
	//device open /dev/mem
	if((mem_fd = open("/dev/mem", O_RDWR | O_SYNC))<0)
	{
		printf("Error : open() /dev/mem\n");
		return -1; 
	}
	//              VirtualAddr                                                  //PhysicalAddr
	gpio_map = mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
	if(gpio_map==MAP_FAILED)
	{
		printf("Error: mmap() : %d\n", (int)gpio_map);
		return -1;
	}
	
	gpio = (volatile unsigned int *)gpio_map; 
	
	GPIO_OUT(gno);
	
	for(i=0;i<5;i++)
	{
		GPIO_SET(gno);
		sleep(1);
		GPIO_CLR(gno);
		sleep(1);
	}
	munmap(gpio_map, GPIO_SIZE);
	
	return 0;
}
