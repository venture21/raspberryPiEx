//================================
// Character device driver example 
// GPIO driver
//================================
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/kernel.h>

#define GPIO_MAJOR 200
#define GPIO_MINOR 0
#define GPIO_DEVICE "gpioled"
#define GPIO_LED   18

//Raspberry Pi3 PHYSICAL I/O Peripherals BaseAddr
#define BCM_IO_BASE 0x3F000000

//GPIO_BaseAddr
#define GPIO_BASE  (BCM_IO_BASE + 0x200000)

//GPIO Function Select Register 0 ~ 5
#define GPIO_IN(g)  (*(gpio+((g)/10)) &=~(7<<(((g)%10)*3)))
#define GPIO_OUT(g) {(*(gpio+((g)/10))&=~(7<<(((g)%10)*3))); \
		     (*(gpio+((g)/10)) |=(1<<(((g)%10)*3)));}

//GPIO Pin Output Set 0 / Clr 0
//                      addr     value 
#define GPIO_SET(g)  (*(gpio+7)=(1<<g))
#define GPIO_CLR(g)  (*(gpio+10)=(1<<g))

#define GPIO_GET(g)  (*(gpio+13)&1<<g)
#define GPIO_SIZE 0xB4

//#define DEBUG

struct cdev gpio_cdev;
volatile unsigned int *gpio;

static struct file_operations gpio_fops={
	.owner   = THIS_MODULE,
//	.read    = gpio_read,
//	.write   = gpio_write,
//	.open    = gpio_open,
//	.release = gpio_close,
};
	


static int __init initModule(void)
{
	dev_t devno;
	int err;
	static void *map;
	int count;
	
	printk("Called initModule()\n");
	
	// 1. 문자디바이스 드라이버를 등록한다.
	devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	register_chrdev_region(devno,1,GPIO_DEVICE);
	
	// 2. 문자 디바이스를 위한 구조체를 초기화 한다.
	cdev_init(&gpio_cdev, &gpio_fops);
	gpio_cdev.owner = THIS_MODULE;
	count=1;
	
	// 3. 문자디바이스를 추가
	err=cdev_add(&gpio_cdev,devno,count);
	if(err<0)
	{
		printk(KERN_INFO "Error: cdev_add()\n");
		return -1;
	}
	
	printk(KERN_INFO "'sudo mknod /dev/%s c %d 0'\n", GPIO_DEVICE, GPIO_MAJOR);
	printk(KERN_INFO "'sudo chmod 666 /dev/%s'\n", GPIO_DEVICE);
	
	//4. 물리메모리 번지로 인자값을 전달하면 가상메모리 번지를 리턴한다.
	map = ioremap(GPIO_BASE, GPIO_SIZE);
	
	if(!map)
	{
		printk(KERN_INFO "Error:mapping GPIO memory\n");
		iounmap(map);
		return -EBUSY;
	} 

#ifdef DEBUG
	printk(KERN_INFO "devno=%d",devno);
#endif
	
	gpio = (volatile unsigned int*)map;
	
	GPIO_OUT(GPIO_LED);
	GPIO_SET(GPIO_LED);
	return 0;
}

static void __exit cleanupModule(void)
{
	dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	
	GPIO_CLR(GPIO_LED);
	
	// 1.문자 디바이스의 등록을 해제한다.
	unregister_chrdev_region(devno,1);
	
	// 2.문자 디바이스의 구조체를 삭제한다.
	cdev_del(&gpio_cdev);
	
	if(gpio)
		iounmap(gpio);
		
	printk("Good-bye!\n");
}


//sudo insmod 호출되는 함수명 정의
module_init(initModule);

//sudo rmmod 호출되는 함수명 정의
module_exit(cleanupModule);

//모듈의 정보
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO Driver Module");
MODULE_AUTHOR("Heejin Park");

