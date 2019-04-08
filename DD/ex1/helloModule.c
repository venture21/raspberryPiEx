#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	printk("Called Hello_init()\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("Called Hello_exit()\n");
}


module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("Heejin Park");

