#include"platform_device.h"

#define MAX_DEVICES	10
#define DEV_MEM_SIZE	512

char kernel_buffer[DEV_MEM_SIZE];

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
	if(count < (DEV_MEM_SIZE - *f_pos))
		count = DEV_MEM_SIZE - *f_pos;

	if(copy_to_user(buf,kernel_buffer,count))
		return -EFAULT;

	*f_pos += count;
	
	return count;

}


static ssize_t my_write(struct file *filp , const  char __user  *buf, size_t count, loff_t *f_pos)
{
	memset(kernel_buffer,0,DEV_MEM_SIZE);
	if(count < (DEV_MEM_SIZE - *f_pos))
		count = DEV_MEM_SIZE - *f_pos;

	if(copy_from_user(kernel_buffer,buf,count))
		return -EFAULT;

	*f_pos += count;

	return 0;
}


static loff_t my_llseek(struct file *filp, loff_t offset, int whence)
{
	if(offset < 0 || offset > DEV_MEM_SIZE)
		return -EINVAL;

	else
	{
		if(whence == SEEK_SET)
			filp->f_pos = offset;

		else if(whence == SEEK_CUR)
			filp->f_pos += offset;


		else if(whence == SEEK_END)
			filp->f_pos = DEV_MEM_SIZE + offset;

		else 
			return -EINVAL;
	}

	return filp->f_pos;





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
	int ret = 0;
	struct device_private_data *dev_prvdata ;
	/*get the platform data*/
	struct platform_device_data *pdev_data = (struct platform_device_data*)dev_get_platdata(&pdev->dev);


	if(!pdev_data)
	{
		pr_info("no platform data available !! ");
		ret = -EINVAL;
		goto out;
	}

	//allocate memory for device_private_data structure
	dev_prvdata = devm_kzalloc(&pdev->dev,sizeof(struct device_private_data),GFP_KERNEL);
	if(!dev_prvdata)
	{
		pr_info("allocate memory failed !!");
		ret = -ENOMEM;
		goto out;
	}

	//save the device private data pointer in platform device structure
	dev_set_drvdata(&pdev->dev,dev_prvdata);

	dev_prvdata->pdata.perm = pdev_data->perm;
	dev_prvdata->pdata.size = pdev_data->size;
	dev_prvdata->pdata.serial_number = pdev_data->serial_number;

	pr_info("serial number : %s\n",dev_prvdata->pdata.serial_number);
	pr_info("permission : %d\n",dev_prvdata->pdata.perm);
	pr_info("size : %d \n",dev_prvdata->pdata.size);

	//allocate memory for device buffer
	dev_prvdata->buffer = devm_kzalloc(&pdev->dev,sizeof(dev_prvdata->pdata.size),GFP_KERNEL);
	if(!dev_prvdata->buffer)
	{
		pr_info("cant allocate memory !! ");
		goto out;
	}

	//get device id
	dev_prvdata->dev_num = drv_data.device_num_base + pdev->id;


	//add device
	cdev_init(&dev_prvdata->cdev,&fops);
	dev_prvdata->cdev.owner = THIS_MODULE;
	ret = cdev_add(&dev_prvdata->cdev,dev_prvdata->dev_num,1);
	if(ret < 0)
	{
		pr_info("register device failed !! ");
		goto out;
	}

	//create device file
	drv_data.drv_device = device_create(drv_data.drv_class,NULL,dev_prvdata->dev_num,NULL,"my_platform_device-%d",pdev->id);

	if(IS_ERR(drv_data.drv_device))
	{
		pr_info("create device file failed !! \n");
		ret = PTR_ERR(drv_data.drv_device);
		cdev_del(&dev_prvdata->cdev);
		goto out;
	}


	pr_info("device is detected !! ");
	return 0;

out:
	pr_info("device probe failed !! ");
	return ret;
}

int pdriver_remove(struct platform_device *pdev)
{
	struct device_private_data *pdev_prvdata = dev_get_drvdata(&pdev->dev);

	device_destroy(drv_data.drv_class,pdev_prvdata->dev_num);
	cdev_del(&pdev_prvdata->cdev);

	drv_data.total--;

	pr_info("device removed !!");
	return 0;
}


struct platform_device_id my_device_id[] =
{
	[0] = {.name = "my_platform_device-0"},
	[1] = {.name = "my_platform_device-1"},
	[2] = {.name = "my_platform_device-2"},
	[3] = {.name = "my_platform_device-3"}

};

struct of_device_id my_id_table[] = 
{
	[0] = {.compatible = "my_platform_device-0"},
	[1] = {.compatible = "my_platform_device-1"},
	[2] = {.compatible = "my_platform_device-2"},
	[3] = {.compatible = "my_platform_device-3"},
	{}
};

struct platform_driver pdriver = 
{
	.probe = pdriver_probe,
	.remove = pdriver_remove,
	.id_table = my_device_id,
	.driver = {
		.name = "my_platform_driver",
		.of_match_table = my_id_table
	
	}
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
