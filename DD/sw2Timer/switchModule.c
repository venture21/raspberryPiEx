//================================
// Character device driver example 
// GPIO driver
//================================
#include <linux/fs.h>		//open(),read(),write(),close() 커널함수
#include <linux/cdev.h>		//register_chrdev_region(), cdev_init()
				//cdev_add(),cdev_del()
#include <linux/module.h>
#include <linux/uaccess.h>	//copy_from_user(), copy_to_user()
#include <linux/gpio.h>		//request_gpio(), gpio_set_value()
				//gpio_get_value(), gpio_free()
#include <linux/interrupt.h>	//gpio_to_irq(), request_irq(), free_irq()
#include <linux/signal.h>	//signal을 사용
#include <linux/sched/signal.h>	//siginfo 구조체를 사용하기 위해
#include <linux/hrtimer.h>	//high-resolution timer
#include <linux/ktime.h>

#include "myswitch.h"

#define MS_TO_NS(x) (x * 1E6L)

struct cdev gpio_cdev;
static char msg[BUF_SIZE] = { 0 };

// switch 2개 irq 번호
static int switch_irq1;
static int switch_irq2;

// switch의 채터링을 위한 flag
static int flag1 = 0;
static int flag2 = 0;

// USER프로그램에 신호 전달
static struct task_struct *task; 	//태스크를 위한 구조체

// 자동으로 노드생성(mknod)
struct class *class;				//class 구조체 
struct device *dev;

// switch 2개의 채터링 방지를 위한 타이머 선언
static struct hrtimer hr_timer;

// 10ms를 측정하기 위한 타이머 선언
static struct hrtimer stopwatch;

// hrTimer를 위한 변수
ktime_t ktime, stime;


enum hrtimer_restart myTimer_callback(struct hrtimer *timer)
{
	if (flag1)
		flag1 = 0;

	printk(KERN_INFO "myTimer_callback\n");

	return HRTIMER_NORESTART;
}

enum hrtimer_restart myStopwatch_callback(struct hrtimer *timer)
{
	ktime_t currtime, interval;
	static int count = 0;
	unsigned long delay_in_ms = 10L;	//100ms
	static struct siginfo sinfo1;

	// USER프로그램에 SIGIO를 전달한다.
	memset(&sinfo1, 0, sizeof(struct siginfo));
	sinfo1.si_signo = SIGIO;
	sinfo1.si_code = SI_USER;
	if (task != NULL)
	{
		//kill()와 동일한 kernel함수
		send_sig_info(SIGIO, &sinfo1, task);
	}
	else
	{
		printk(KERN_INFO "Error: USER PID\n");
	}

	//sw2가 입력되면 stopwatchTimer종료
	if (flag2)
	{
		flag2 = 0;
		pr_info("stopwatch Time=%d\n", count);
		count = 0;
		
		return HRTIMER_NORESTART;
	}
	else
	{
		count++;

		currtime = ktime_get();
		interval = ktime_set(0, MS_TO_NS(delay_in_ms));
		hrtimer_forward(timer, currtime, interval);
		return HRTIMER_RESTART;
	}

}

//switch 2개를 인터럽트 소스로 사용
static irqreturn_t isr_func(int irq, void *data)
{
	//unsigned long delay_in_ms = 50L;	//50ms
	//MS_TO_NS(delay_in_ms)
	unsigned long expireTime = 50000000L;	 //50ms unit:ns
	unsigned long stopwatchTime = 10000000L; //100ms unit:ns

	static int count=0;
	
	static struct siginfo sinfo;

	// sw1이 눌렸을 경우 
	if (irq == switch_irq1)
	{
		if (!flag1)
		{
			flag1 = 1;
			count = 0;
			//-------------------------------------------------------------------

			printk(KERN_INFO "start switch\n");
			//kitme_set(설정초,설정나노초);
			ktime = ktime_set(0, expireTime);
			hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
			//-------------------------------------------------------------------
			
			//-------------------------------------------------------------------
			printk(KERN_INFO "stopWatch is start\n");
			//kitme_set(설정초,설정나노초);
			stime = ktime_set(0, stopwatchTime);
			hrtimer_start(&stopwatch, stime, HRTIMER_MODE_REL);
			//-------------------------------------------------------------------
			//스위치가 눌렸을 때 응용프로그램에게 SIGUSR1를 전달한다.
			//sinfo구조체를 0으로 초기화한다.
			memset(&sinfo, 0, sizeof(struct siginfo));
			sinfo.si_signo = SIGUSR1;
			sinfo.si_code = SI_USER;
			if (task != NULL)
			{
				//kill()와 동일한 kernel함수
				send_sig_info(SIGUSR1, &sinfo, task);
			}
			else
			{
				printk(KERN_INFO "Error: USER PID\n");
			}
			count++;
			printk(KERN_INFO " Called isr_func():%d\n", count);
		}
	} //sw2이 눌렸을 경우
	else if(irq == switch_irq2)
	{
		if (!flag2)
		{
			flag2 = 1;

			printk(KERN_INFO "stop switch\n");
			ktime = ktime_set(0, expireTime);
			hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
			
			//스위치가 눌렸을 때 응용프로그램에게 SIGUSR1를 전달한다.
			//sinfo구조체를 0으로 초기화한다.
			memset(&sinfo, 0, sizeof(struct siginfo));
			sinfo.si_signo = SIGUSR2;
			sinfo.si_code = SI_USER;
			if (task != NULL)
			{
				//kill()와 동일한 kernel함수
				send_sig_info(SIGUSR2, &sinfo, task);
			}
			else
			{
				printk(KERN_INFO "Error: USER PID\n");
			}
			count++;
			printk(KERN_INFO " Called isr_func():%d\n", count);
		}
	}
	return IRQ_HANDLED;
}


static int gpio_open(struct inode *inod, struct file *fil)
{
	//모듈의 사용 카운트를 증가 시킨다.
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "GPIO Device opened\n");
	return 0;
}


static int gpio_close(struct inode *inod, struct file *fil)
{
	//모듈의 사용 카운트를 감소 시킨다.
	module_put(THIS_MODULE);
	printk(KERN_INFO " GPIO Device closed\n");
	return 0;
}


static ssize_t gpio_write(struct file *fil, const char *buff, size_t len, loff_t *off)
{
	int count;
	char *endptr;
	pid_t pid;

	memset(msg, 0, BUF_SIZE);

	count = copy_from_user(msg, buff, len);
	
	// msg문자열을 숫자로 변환
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
	return count;
}

static struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
	.write = gpio_write,
	//.read = gpio_read,
	.open = gpio_open,
	.release = gpio_close,
};


//================================================
// 모듈 생성시 초기화 동작
// 1.문자 디바이스 드라이버 등록 
//================================================
static int __init initModule(void)
{
	dev_t devno;
	int err;
	int count;

	printk("Called initModule()\n");

	// 1. 문자디바이스 드라이버를 등록한다.
	devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	register_chrdev_region(devno, 1, GPIO_DEVICE);

	// 2. 문자 디바이스를 위한 구조체를 초기화 한다.
	cdev_init(&gpio_cdev, &gpio_fops);
	gpio_cdev.owner = THIS_MODULE;
	count = 1;

	// 3. 문자디바이스를 추가
	err = cdev_add(&gpio_cdev, devno, count);
	if (err < 0)
	{
		printk(KERN_INFO "Error: cdev_add()\n");
		return -1;
	}

	//class를 생성한다.
	class = class_create(THIS_MODULE, GPIO_DEVICE);
	if (IS_ERR(class))
	{
		err = PTR_ERR(class);
		printk(KERN_INFO "class_create error %d\n", err);

		cdev_del(&gpio_cdev);
		unregister_chrdev_region(devno, 1);
		return err;
	}

	//노드를 자동으로 만들어준다.
	dev = device_create(class, NULL, devno, NULL, GPIO_DEVICE);
	if (IS_ERR(dev))
	{
		err = PTR_ERR(dev);
		printk(KERN_INFO "device create error %d\n", err);
		class_destroy(class);
		cdev_del(&gpio_cdev);
		unregister_chrdev_region(devno, 1);
		return err;
	}

	printk(KERN_INFO "'sudo chmod 666 /dev/%s'\n", GPIO_DEVICE);

	// 현재 GPIO_SW1이 사용중인지 확인하고 사용권한 획득
	err = gpio_request(GPIO_SW1, "SW1");
	if (err == -EBUSY)
	{
		printk(KERN_INFO "Error gpio_request LED\n");
		return -1;
	}

	switch_irq1 = gpio_to_irq(GPIO_SW1);
	err = request_irq(switch_irq1, isr_func, IRQF_TRIGGER_RISING, "switch1", NULL);
	if (err)
	{
		printk(KERN_INFO "Error request_irq\n");
		return -1;
	}

	// 현재 GPIO_SW2이 사용중인지 확인하고 사용권한 획득
	err = gpio_request(GPIO_SW2, "SW2");
	if (err == -EBUSY)
	{
		printk(KERN_INFO "Error gpio_request SW\n");
		return -1;
	}

	switch_irq2 = gpio_to_irq(GPIO_SW2);
	err = request_irq(switch_irq2, isr_func, IRQF_TRIGGER_RISING, "switch2", NULL);
	if (err)
	{
		printk(KERN_INFO "Error request_irq\n");
		return -1;
	}

	// 채터링 방지 타이머 생성 및 초기화
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = &myTimer_callback;
	   	 
	// stopwatch 타이머 생성 및 초기화
	hrtimer_init(&stopwatch, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	stopwatch.function = &myStopwatch_callback;

	return 0;
}

static void __exit cleanupModule(void)
{
	dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);

	// 0. 생성했던 노드를 제거한다.
	device_destroy(class, devno);
	class_destroy(class);

	// 1.문자 디바이스의 등록을 해제한다.
	unregister_chrdev_region(devno, 1);

	// 2.문자 디바이스의 구조체를 삭제한다.
	cdev_del(&gpio_cdev);

	//request_irq에서 받아온 사용권한을 반납한다.
	free_irq(switch_irq1, NULL);
	free_irq(switch_irq2, NULL);

	//gpio_request()에서 받아온 사용권한을 반납한다.
	gpio_free(GPIO_SW1);
	gpio_free(GPIO_SW2);

	//hr-Timer 취소
	hrtimer_cancel(&stopwatch);
	hrtimer_cancel(&hr_timer);
	
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

