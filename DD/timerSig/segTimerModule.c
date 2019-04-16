//================================
// Character device driver example 
// GPIO driver
//================================
#include <linux/fs.h>		//open(),read(),write(),close() 커널함수
#include <linux/cdev.h>		//register_chrdev_region(), cdev_init()
				//cdev_add(),cdev_del()
#include <linux/module.h>
//#include <linux/io.h>		//ioremap(), iounmap()
#include <linux/uaccess.h>	//copy_from_user(), copy_to_user()
#include <linux/gpio.h>		//request_gpio(), gpio_set_value()
				//gpio_get_value(), gpio_free()
#include <linux/interrupt.h>	//gpio_to_irq(), request_irq(), free_irq()
#include <linux/timer.h>	//init_timer(), add_timer(), del_timer()
#include <linux/signal.h>	//signal을 사용
#include <linux/sched/signal.h>	//siginfo 구조체를 사용하기 위해
#include <linux/hrtimer.h>
#include <linux/ktime.h>


#define SEG_MAJOR	200
#define SEG_MINOR	0
#define DEVICE_NAME	"segtimer"
#define BLK_SIZE	100
//#define DEBUG

struct cdev seg_cdev;
struct class *class;
struct device *dev;

static char msg[BLK_SIZE] = { 0 };
static struct task_struct *task; 	//태스크를 위한 구조체
pid_t pid;
char pid_valid;
int iSEC;
int iMSEC;

static int seg_open(struct inode *, struct file *);
static int seg_close(struct inode *, struct file *);
static ssize_t seg_read(struct file *, char *buff, size_t, loff_t *);
static ssize_t seg_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations seg_fops = {
	.owner = THIS_MODULE,
	.read = seg_read,
	.write = seg_write,
	.open = seg_open,
	.release = seg_close,
};

#define MS_TO_NS(x) (x * 1E6L)

static struct hrtimer hr_timer;

enum hrtimer_restart my_hrtimer_callback(struct hrtimer *timer)
{
	ktime_t currtime, interval;
	unsigned long delay_in_ms = 10L;	//100ms

	//10초후 종료
	if (iSEC == 1)
	{
		pr_info("my_hrtimer_callback is done (%ld).\n", jiffies);
		return HRTIMER_NORESTART;
	}
	else
	{
		if (iMSEC == 99)
		{
			iSEC++;
			iMSEC = 0;
		}
		else
			iMSEC++;

		pr_info("Time:%d.%d\n", iSEC,iMSEC);
		
		currtime = ktime_get();
		interval = ktime_set(0, MS_TO_NS(delay_in_ms));
		hrtimer_forward(timer, currtime, interval);
		return HRTIMER_RESTART;
	}
}

static int seg_open(struct inode *inod, struct file *fil)
{
	//모듈의 사용 카운트를 증가 시킨다.
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "7-Segment Device opened\n");
	return 0;
}


static int seg_close(struct inode *inod, struct file *fil)
{
	//모듈의 사용 카운트를 감소 시킨다.
	module_put(THIS_MODULE);
	printk(KERN_INFO "7-Segment Device closed\n");
	return 0;
}

static ssize_t seg_read(struct file *fil, char *buff, size_t len, loff_t *off)
{
	int count;

	//커널영역에 있는 msg문자열을 유저영역의 buff주소로 복사 
	count = copy_to_user(buff, msg, strlen(msg) + 1);

	printk(KERN_INFO "7-segment Device read:%s\n", msg);

	return count;
}


static ssize_t seg_write(struct file *fil, const char *buff, size_t len, loff_t *off)
{
	int count;
	char *endptr;

	memset(msg, 0, BLK_SIZE);

	count = copy_from_user(msg, buff, len);

	//pidstr문자열을 숫자로 변환
	pid = simple_strtol(msg, &endptr, 10);
	printk(KERN_INFO "pid=%d\n", pid);

	if (endptr != NULL)
	{
		// pid값을 통해 task_struct구조체의 주소값을 확인 
		task = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (task == NULL)
		{
			printk(KERN_INFO "Error:Can't find PID from user APP\n");
			return 0;
		}
	}

	printk(KERN_INFO "7-segment Device write:%s\n", msg);
	return count;
}

static int __init initModule(void)
{
	dev_t devno;
	int err;
	int count;
	ktime_t ktime;
	unsigned long delay_in_ms = 10L;  //10ms

	printk("Called initModule()\n");

	// 1. 문자디바이스 드라이버를 등록한다.
	devno = MKDEV(SEG_MAJOR, SEG_MINOR);
	register_chrdev_region(devno, 1, DEVICE_NAME);

	// 2. 문자 디바이스를 위한 구조체를 초기화 한다.
	cdev_init(&seg_cdev, &seg_fops);
	seg_cdev.owner = THIS_MODULE;
	count = 1;

	// 3. 문자디바이스를 추가
	err = cdev_add(&seg_cdev, devno, count);
	if (err < 0)
	{
		printk(KERN_INFO "Error: cdev_add()\n");
		return -1;
	}

	//class를 생성한다.
	class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(class))
	{
		err = PTR_ERR(class);
		printk(KERN_INFO "class_create error %d\n", err);

		cdev_del(&seg_cdev);
		unregister_chrdev_region(devno, 1);
		return err;
	}

	//노드를 자동으로 만들어준다.
	dev = device_create(class, NULL, devno, NULL, DEVICE_NAME);
	if (IS_ERR(dev))
	{
		err = PTR_ERR(dev);
		printk(KERN_INFO "device create error %d\n", err);
		class_destroy(class);
		cdev_del(&seg_cdev);
		unregister_chrdev_region(devno, 1);
		return err;
	}

	//printk(KERN_INFO "'sudo mknod /dev/%s c %d 0'\n", DEVICE_NAME, SEG_MAJOR);
	printk(KERN_INFO "'sudo chmod 666 /dev/%s'\n", DEVICE_NAME);

	pr_info("HR Timer module installing\n");

	//===============================================
	// ktime은 ns를 기본 단위로 사용하여 10ms에 
	// 10^6을 곱해준다.
	// * ktime = ktime_set(0, 100 * 1000 * 1000);
	// * 10 ms = 10 * 1000 * 1000 ns
	//===============================================	 
	// 10ms 시간을 ktime에 저장
	ktime = ktime_set(0, MS_TO_NS(delay_in_ms));

	//hr_timer를 초기화
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

	//hr_timer의 타임 만료시 호출할 함수 지정 
	hr_timer.function = &my_hrtimer_callback;
	pr_info("Starting timer to fire in %ldms (%ld)\n", delay_in_ms, jiffies);

	//hr_timer 시작
	hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);

	return 0;
}

static void __exit cleanupModule(void)
{
	int ret;
	dev_t devno = MKDEV(SEG_MAJOR, SEG_MINOR);

	// 0.hrTimer cancel
	ret = hrtimer_cancel(&hr_timer);
	if (ret)
		pr_info("The timer was still in use...\n");

	pr_info("HR Timer module uninstalling\n");

	// 1.문자 디바이스의 등록을 해제한다.
	unregister_chrdev_region(devno, 1);

	// 2.문자 디바이스의 구조체를 삭제한다.
	cdev_del(&seg_cdev);
	
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

