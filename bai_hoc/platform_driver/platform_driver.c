#include<linux/module.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/platform_device.h>
#include<linux/slab.h>
#include"platform_device.h"

#define MAX_DEVICES	10

struct drv_private_data drv_data;



static int  my_open(struct inode *inode , struct file *filp)
{

	return 0;
}

static int my_release(struct inode *inode , struct file *filp)
{
	return 0;
}

static ssize_t  my_read(struct file *filp, __user char* buf , size_t count, loff_t *f_pos)
{

	return 0;

}


static ssize_t my_write(struct file *filp , const  char __user  *buf, size_t count, loff_t *f_pos)
{

	return 0;
}


static loff_t my_llseek(struct file *filp, loff_t offset, int whence)
{

	return 0;
}




struct file_operations fops = 
{
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.llseek = my_llseek,
	.owner = THIS_MODULE
};


int pdriver_probe(struct platform_device *pdev)
{
	int ret;
	struct device_private_data *dev_prdata;
	/*get the platform data*/
	struct platform_device_data *pdata = (struct platform_device_data*)dev_get_platdata(&pdev->dev);
	if(!pdata)
	{
		pr_info("no platform data available !! ");
		ret = -EINVAL;
		goto out;
	}

	dev_prdata = kzalloc(sizeof(struct device_private_data),GFP_KERNEL);
	if(!dev_prdata)
	{
		pr_info("allocate memory failed !!");
		ret = -ENOMEM;
		goto out;
	}

	dev_prdata->pdata.perm = pdata->perm;
	dev_prdata->pdata.size = pdata->size;
	dev_prdata->pdata.serial_number = pdata->serial_number;

	pr_info("serial number : %s\n",dev_prdata->pdata.serial_number);
	pr_info("permission : %d\n",dev_prdata->pdata.perm);
	pr_info("size : %d \n",dev_prdata->pdata.size);


	/*dynamically allocate memory for the device*/


	pr_info("device is detected !! ");
	return 0;

out:
	pr_info("device probe failed !! ");
	return ret;
}

int pdriver_remove(struct platform_device *pdev)
{
	pr_info("device removed !!");
	return 0;
}


struct platform_driver pdriver = 
{
	.probe = pdriver_probe,
	.remove = pdriver_remove,
	.driver = {.name = "my_platform_device"}
};


static int __init platform_driver_init(void)
{
	int ret;
	ret = alloc_chrdev_region(&drv_data.device_num_base,0,MAX_DEVICES,"my_devices");
	if(ret < 0)
	{
		pr_err("allocate device number failed !!!");
		return ret;
	}

	drv_data.drv_class = class_create(THIS_MODULE,"my_class");
	if(IS_ERR(drv_data.drv_class))
	{
		pr_err("class creation failed !!");
		ret = PTR_ERR(drv_data.drv_class);
		unregister_chrdev_region(drv_data.device_num_base,MAX_DEVICES);
		return ret;
	}

	platform_driver_register(&pdriver);
	pr_info("the module loaded successfully !!! ");
	return 0;
}


static void __exit platform_driver_exit(void)
{
	platform_driver_unregister(&pdriver);
	class_destroy(drv_data.drv_class);
	unregister_chrdev_region(drv_data.device_num_base,MAX_DEVICES);
	pr_info("the module unloaded !!!");
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Duy My");
