#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>		//file operations
#include <linux/cdev.h>		//for makes cdev available
#include <linux/semaphore.h>	//mutex will be used so we'll need this
#include <asm/uaccess.h>	//allow user access ie copy_to_user etc.
#include <linux/device.h>


MODULE_LICENSE("GPL");
//step 1: Create a "fake" device to read and write to
struct fake_device {
	char data[100];  //testing purposes only
	struct semaphore sem; //again for testing purposes
} virtual_device;

//step 2: Create global variables

struct cdev *my_c_dev;
struct class * cl;
int majNum;
int ret ;
dev_t dev_num;  //holds the major number the the kernel assigns us
#define DEVICE_NAME	"ECE_331_Device"

//step 7:
int device_open(struct inode *inode, struct file *filp){	//This function will also reject other programs/processes if the kernel is current busy with a program/process
	if(down_interruptible(&virtual_device.sem) != 0){	//if the file is being written we must wait
	printk(KERN_ALERT "error: device is busy\n");
	return -1;
	}
	printk(KERN_INFO "ECE 331: opened device\n");
	return 0;
}

//step 8
ssize_t device_read(struct file *filp, char* bufStoreData, size_t bufCount, loff_t* curOffset)
{
	printk(KERN_INFO "ECE 331: Reading from device\n");
	ret = copy_to_user(bufStoreData, virtual_device.data,bufCount);
	return ret;
}
//setp 9
ssize_t device_write(struct file* filp, const char* bufSourceData, size_t bufCount, loff_t* curOffset)
{
	printk(KERN_INFO "ECE 331: Writing to device\n");
	ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
	return ret;
}

//step10
int device_close(struct inode *inode, struct file *filp)
{
	up(&virtual_device.sem);
	printk(KERN_INFO "ECE 331: closed device\n");
	return 0;
}

static long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	switch(cmd){
		case READ:
		printk(KERN_INFO "ECE 331: Reading from device\n");
		ret = copy_to_user((char *)arg, &virtual_device.data, sizeof(virtual_device.data));
	//	return ret;
		break;

		default:
			return -EINVAL;
	}

	return 0;
}
//step 6:
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.write = device_write,
	.read = device_read,
	.unlocked_ioctl = device_ioctl
};
//step 3: Create and register the new device with the system

static int driver_entry(void)
{
	if((ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME)) < 0){
		printk(KERN_ALERT "error: failed to allocate a major number.\n");
		return ret;
}
	majNum = MAJOR(dev_num);	//get major number assigned by kerenl for our purposes
	if((cl = class_create(THIS_MODULE, DEVICE_NAME)) == NULL){	//create a class so we can make a new device
		unregister_chrdev_region(dev_num, 1);
		printk(KERN_ALERT "error: failed to create a class for %s",DEVICE_NAME);
		return -1;
	}
	if(device_create(cl, NULL, dev_num, NULL, DEVICE_NAME) == NULL){
		class_destroy(cl);
		unregister_chrdev_region(dev_num, 1);
		printk(KERN_ALERT "error: failed to create device.");
		return -1;
	}
	printk(KERN_INFO "ECE 331: Major number is %d\n",majNum);
	printk(KERN_INFO "ECE 331: use mknod /dev/%s c %d 0 for device file\n",DEVICE_NAME, majNum);
	
	my_c_dev = cdev_alloc();	//create and initalize the new cdev
	my_c_dev->ops = &fops;
	my_c_dev->owner = THIS_MODULE;	//will need to look into the struct to see if this will change permisions.
	ret = cdev_add(my_c_dev, dev_num, 1);	//need to look up what the last argument does.
	if(ret < 0){
		printk(KERN_ALERT "error: unable to add cdev to kernel\n");
		return ret;
	}

	//step 4: initalize a semaphore
	sema_init(&virtual_device.sem,1);	//need to look this up.
	
	return 0;
}

static void driver_exit(void)
{
	device_destroy(cl, dev_num);
	class_destroy(cl);
	cdev_del(my_c_dev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "ECE 331: unloaded module\n");
}

module_init(driver_entry);
module_exit(driver_exit);
