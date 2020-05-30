#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/interrupt.h>
#include<linux/ioctl.h>
#include<linux/delay.h>

void task_func(unsigned long data);
char kernel_buffer[1000];
dev_t dev;
struct cdev *my_cdev;
struct class *my_class;
int IRQ_NUM = 1;
volatile int count = 0;
DECLARE_TASKLET(task,task_func,1);

irqreturn_t irq_handler(int irq, void* dev_id)
{
	tasklet_schedule(&task);
	count++;
	pr_info("the keyboard has pressed %d times !!! \n",count);
	pr_info("the interrupt handler is done !!! \n");
	return IRQ_HANDLED;
}


void task_func(unsigned long data)
{
	pr_info("this is tasklet function !!! \n");
	msleep(10);
}



static int my_open(struct inode *inode , struct file *filp)
{
	pr_info("the file is opened !!! \n");
	return 0;
}

static int my_release(struct inode *inode ,struct file *filp)
{
	pr_info("the file is closed !!! \n");
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	ssize_t ret;
	if(sizeof(kernel_buffer) <= *off)
		ret = 0;

	else
	{
		if((sizeof(kernel_buffer) - *off) > len )
			ret = len;
		else
			ret = sizeof(kernel_buffer) - *off;
	}

	if(copy_to_user(buf,kernel_buffer,ret))
		ret = -EINVAL;

	else
		*off+=ret;

	return ret;
}

ssize_t my_write(struct file *filp, const char *buf , size_t len, loff_t *off)
{
	ssize_t ret;
	if((sizeof(kernel_buffer)- *off) < len)
		ret = -ENOSPC;

	else
	{
		ret = len;
		memset(kernel_buffer,0,sizeof(kernel_buffer));
		if(copy_from_user(kernel_buffer,buf,ret))
			ret = -EINVAL;

		else
			*off+=ret;
	}

	return ret;
}


static long my_ioctl(struct file *filp, unsigned int cmd , unsigned long value)
{
	pr_info("this is ioclt function !!! \n");
	return 0;
}


struct file_operations my_ops =
{
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl
};


static int __init char_driver_init(void)
{
	if(alloc_chrdev_region(&dev,0,1,"my_dev")<0)
	{
		pr_info("cant allocate the major number !!! \n");
		return EFAULT;
	}

	my_cdev = cdev_alloc();
	my_cdev->owner = THIS_MODULE;
	my_cdev->ops = &my_ops;
	if(cdev_add(my_cdev,dev,1)<0)
	{
		pr_info("cant add a device into system !! \n");
		goto r_class;
	}

	if((my_class = class_create(THIS_MODULE,"my_class"))== NULL)
	{
		pr_info("cant create class device !! \n");
		goto r_class;
	}

	if(device_create(my_class,NULL,dev,NULL,"my_device")== NULL)
	{
		pr_info("cant create device file !! \n");
		goto r_device;
	}

	if(request_irq(IRQ_NUM,irq_handler,IRQF_SHARED,"my_interrupt",(void*)irq_handler) != 0)
	{
		pr_info("cant regisered an interrupt !! \n ");
		return EFAULT;
	}

	return 0;

r_class:
	unregister_chrdev_region(dev,1);
r_device:
	class_destroy(my_class);

	return -1;

}

static void __exit char_driver_exit(void)
{

	tasklet_kill(&task);
	free_irq(IRQ_NUM,(void*)irq_handler);
	cdev_del(my_cdev);
	device_destroy(my_class,dev);
	class_destroy(my_class);
	unregister_chrdev_region(dev,1);
	pr_info("the module is unloaded !!! \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
MODULE_LICENSE("GPL");
