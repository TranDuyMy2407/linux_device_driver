#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>

struct cdev *my_cdev;
struct class *my_class;
struct device *my_device;
dev_t dev;
char buffer[1000];



static int my_open(struct inode *inode, struct file *filp)
{
	pr_info("the file is open !!! \n");
	return 0;
}


static int my_release(struct inode *inode ,struct file *filp)
{
	pr_info("the file is closed !!! \n");
	return 0;
}


static ssize_t my_read(struct file *filp , char __user *buf , size_t len, loff_t *off)
{
	ssize_t ret;
	if(sizeof(buffer) <= *off)
		ret = 0;

	else
	{
		
		if((sizeof(buffer) - *off) > len)
			ret = len;
		
		else
			ret = sizeof(buffer) - *off;


		if(copy_to_user(buf,buffer,ret))
        	        ret = -EFAULT;

       		else
        	        *off += ret;
	

		
	}


	return ret;

}

static ssize_t my_write(struct file *filp ,const  char *buf, size_t len, loff_t *off)
{
	ssize_t ret;
	memset(buffer,0,1000);
	
	if((sizeof(buffer) - *off) < len)
		ret = -ENOSPC;

	else
	{
		if(copy_from_user(buffer,buf,len))
			ret = -EFAULT;

		else
		{
			ret = len;
			*off += ret;
		}
	}

	return ret;
}




static struct file_operations fops =
{
        .open = my_open,
        .release = my_release,
        .read = my_read,
        .write = my_write
};



static int __init char_driver_init(void)
{


	if(alloc_chrdev_region(&dev,0,1,"my_device")<0)
	{
		pr_info("cant allocate the major number !!! \n");
		return -1;
	}

	my_cdev = cdev_alloc();	
	my_cdev->owner = THIS_MODULE;
	my_cdev->ops = &fops;

	if(cdev_add(my_cdev,dev,1) < 0)
	{
		pr_info("can not add a file into the system !!! \n");
		goto r_class;
	}		
	my_class = class_create(THIS_MODULE,"my_class");
	if(my_class == NULL)
	{
		pr_info("cant create class device !!! \n");
		goto r_class;
	}


	my_device = device_create(my_class,NULL,dev,NULL,"my_device");
	if(my_device == NULL)
	{
		pr_info("cant create the device file !!! \n");
		goto r_device;
	}

	
r_device:
	class_destroy(my_class);
r_class:
	unregister_chrdev_region(dev,1);
	

	return 0;

}


static void __exit char_driver_exit(void)
{
	cdev_del(my_cdev);
	device_destroy(my_class,dev);
	class_destroy(my_class);
	unregister_chrdev_region(dev,1);
	pr_info("the module has  removed !!! \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
MODULE_LICENSE("GPL");
