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
#include "myswitch.h"

struct cdev gpio_cdev;
static char msg[BUF_SIZE] = { 0 };
static int switch_irq1;
static int switch_irq2;

static struct task_struct *task; 	//태스크를 위한 구조체
struct class *class;				//class 구조체 
struct device *dev;

pid_t pid;

//switch 2개를 인터럽트 소스로 사용
static irqreturn_t isr_func(int irq, void *data)
{
	//IRQ발생 & LED가 OFF일때 
	static int count;
	static int flag = 0;

	static struct siginfo sinfo;

	if (!flag)
	{
		flag = 1;
		if ((irq == switch_irq) && !gpio_get_value(GPIO_LED))
		{
			gpio_set_value(GPIO_LED, 1);

			//스위치가 눌렸을 때 응용프로그램에게 SIGIO를 전달한다.
			//sinfo구조체를 0으로 초기화한다.
			memset(&sinfo, 0, sizeof(struct siginfo));
			sinfo.si_signo = SIGIO;
			sinfo.si_code = SI_USER;
			if (task != NULL)
			{
				//kill()와 동일한 kernel함수
				send_sig_info(SIGIO, &sinfo, task);
			}
			else
			{
				printk(KERN_INFO "Error: USER PID\n");
			}

		}
		else //IRQ발생 & LED ON일때
			gpio_set_value(GPIO_LED, 0);

		printk(KERN_INFO " Called isr_func():%d\n", count);
		count++;
	}
	else
	{
		flag = 0;
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

static ssize_t gpio_read(struct file *fil, char *buff, size_t len, loff_t *off)
{
	int count;
	// <linux/gpio.h>파일에 있는 gpio_get_value()를 통해
	// gpio의 상태값을 읽어온다. 
	if (gpio_get_value(GPIO_LED))
		msg[0] = '1';
	else
		msg[1] = '0';

	// 이 데이터가 커널에서 온 데이터임을 표기한다.
	strcat(msg, " from kernel");

	//커널영역에 있는 msg문자열을 유저영역의 buff주소로 복사 
	count = copy_to_user(buff, msg, strlen(msg) + 1);

	printk(KERN_INFO "GPIO Device read:%s\n", msg);

	return count;
}


static ssize_t gpio_write(struct file *fil, const char *buff, size_t len, loff_t *off)
{
	int count;
	char *cmd, *str;
	char *sep = ":";
	char *endptr, *pidstr;

	memset(msg, 0, BLK_SIZE);

	count = copy_from_user(msg, buff, len);
	str = kstrdup(msg, GFP_KERNEL);
	cmd = strsep(&str, sep);
	pidstr = strsep(&str, sep);
	//cmd를 문자열로 인식시키기 위해서
	cmd[1] = '0';
	printk(KERN_INFO "cmd:%s, pid:%s\n", cmd, pidstr);

	if (!strcmp(cmd, "0"))
	{
		del_timer_sync(&timer);
	}
	else
	{
		init_timer(&timer);
		timer.function = timer_func;
		timer.data = 1L;	//timer_func으로 전달하는 인자값
		timer.expires = jiffies + (1 * HZ); //1초 뒤에 타이머 만료
		add_timer(&timer);
	}
	if (!strcmp(cmd, "end"))
	{
		pid_valid = 0;
	}
	else
	{
		pid_valid = 1;
	}

	//pidstr문자열을 숫자로 변환
	pid = simple_strtol(pidstr, &endptr, 10);
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

	gpio_set_value(GPIO_LED, (strcmp(msg, "0")));
	printk(KERN_INFO "GPIO Device write:%s\n", msg);
	return count;
}

//================================================
// 모듈 생성시 초기화 동작
// 1.문자 디바이스 드라이버 등록 
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
	   	 
	return 0;
}

static void __exit cleanupModule(void)
{
	dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);

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
	
	printk("Good-bye!\n");
}


static struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
	.read = gpio_read,
	.write = gpio_write,
	.open = gpio_open,
	.release = gpio_close,
};



//sudo insmod 호출되는 함수명 정의
module_init(initModule);

//sudo rmmod 호출되는 함수명 정의
module_exit(cleanupModule);

//모듈의 정보
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO Driver Module");
MODULE_AUTHOR("Heejin Park");

