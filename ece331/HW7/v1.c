#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>		//file operations
#include <linux/cdev.h>		//for makes cdev available
#include <linux/semaphore.h>	//mutex will be used so we'll need this
#include <asm/uaccess.h>	//allow user access ie copy_to_user etc.
#include <linux/device.h>
#include <linux/types.h>
#define DEVICE_NAME	"ECE_331_Device"
#include "v1.h"

static int device_open(struct inode *inode, struct file *filp);
static int device_close(struct inode *inode, struct file *filp);
static long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg);
static char *module_perm(struct device *dev, umode_t *mode); 


// Create global variables
struct cdev *my_c_dev;
struct mod_data{
	struct class *module_class;
} module_data;
struct semaphore sem;
int majNum;
int ret;
dev_t dev_num;  //holds the major number the the kernel assigns us
uint64_t data = 0; //hold the vaule to be read and written

int device_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "ECE 331: device was opened\n");
	return 0;
}


int device_close(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "ECE 331: device was closed\n");
	return 0;
}


long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	virtual_device v;
	switch(cmd){
		case READ_DEVICE:
		v.data = data;
		printk(KERN_INFO "ECE 331: device was read\n");
		if(copy_to_user((virtual_device *)arg, &v, sizeof(virtual_device))){
			return -EACCES;
		}
		up(&sem); //allow another process to write to the device now.
		break;

		case WRITE_DEVICE:
			if(down_interruptible(&sem) != 0){	//if the file has already been written we must wait
			printk(KERN_ALERT "error: The device needs to be read first before it can be written again\n");
			return -1;
			}
		if(copy_from_user(&v, (virtual_device *)arg, sizeof(virtual_device))){
			return -EACCES;
		}
		data = v.data;
		printk(KERN_INFO "ECE 331: device was written\n");
		break;

		default:
			return -EINVAL;
	}

	return 0;
}


//function to set permissons of device
char *module_perm(struct device *dev, umode_t *mode)
{
	if(mode) *mode = 0666;
	return NULL;
}


struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.unlocked_ioctl = device_ioctl
};


//Create and register the new device with the system
static int driver_entry(void)
{
	if((ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME)) < 0){
		printk(KERN_ALERT "error: failed to allocate a major number.\n");
		return ret;
}
	majNum = MAJOR(dev_num);	//get major number assigned by kerenl for our purposes
	if((module_data.module_class = class_create(THIS_MODULE, DEVICE_NAME)) == NULL){	//create a class so we can make a new device
		unregister_chrdev_region(dev_num, 1);
		printk(KERN_ALERT "error: failed to create a class for %s",DEVICE_NAME);
		return -1;
	}
	printk(KERN_INFO "before the function call.\n");

	module_data.module_class->devnode = module_perm; //allow everyone to read and write to the device

	if(device_create(module_data.module_class, NULL, dev_num, NULL, DEVICE_NAME) == NULL){
		class_destroy(module_data.module_class);
		unregister_chrdev_region(dev_num, 1);
		printk(KERN_ALERT "error: failed to create device.");
		return -1;
	}
	
	if((my_c_dev = cdev_alloc()) == NULL){	//create and initalize the new cdev
		device_destroy(module_data.module_class, dev_num);
		class_destroy(module_data.module_class);
		cdev_del(my_c_dev);
		unregister_chrdev_region(dev_num, 1);
		return -1;
	}
	my_c_dev->ops = &fops;
	my_c_dev->owner = THIS_MODULE;
	if((ret = cdev_add(my_c_dev, dev_num, 1) < 0)){
		printk(KERN_ALERT "error: unable to add cdev to kernel\n");
		return ret;
	}

	//initalize our mutex semaphore
	sema_init(&sem,1);
	return 0;
}


static void driver_exit(void)
{
	device_destroy(module_data.module_class, dev_num);
	class_destroy(module_data.module_class);
	cdev_del(my_c_dev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "ECE 331: unloaded module\n");
}

module_init(driver_entry);
module_exit(driver_exit);
MODULE_LICENSE("GPL");
