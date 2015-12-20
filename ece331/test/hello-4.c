#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#define DRIVER_AUTHOR "Scott Edgerly"
#define DRIVER_DESC "Sample driver code from http://www.tldp.org/LDP/lkmpg/2.6/lkmpg.pdf for learning purposes."

static int hello4_data __initdata = 4;

static int __init hello_4_init(void)
{
	printk(KERN_INFO "Hello World %d\n", hello4_data);
	return 0;
}

static void __exit hello_4_exit(void)
{
	printk(KERN_INFO "Goodbye World 4\n");
}

module_init(hello_4_init);
module_exit(hello_4_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
