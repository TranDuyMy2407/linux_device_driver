#include<linux/module.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

#define NO_OF_DEVICES		4

/*size device memory*/

#define DEV_MEM_SIZE_DEV1	1024
#define DEV_MEM_SIZE_DEV2	512
#define DEV_MEM_SIZE_DEV3	1024
#define DEV_MEM_SIZE_DEV4	512


/*device permission*/
#define RDONLY			0x1
#define WRONLY			0x2
#define RDWR			0x3

/*device buffer*/
char device1_buffer[DEV_MEM_SIZE_DEV1];
char device2_buffer[DEV_MEM_SIZE_DEV2];
char device3_buffer[DEV_MEM_SIZE_DEV3];
char device4_buffer[DEV_MEM_SIZE_DEV4];

/* device private data*/
struct device_private_data
{
	char *buffer;
	unsigned int size;
	const char *serial_number;
	unsigned int perm;
	struct cdev cdev;
};


/*driver private data*/
struct driver_private_data
{
	unsigned int total;
	dev_t dev_number;
	struct class *drv_class;
	struct device *drv_device;
	struct device_private_data  dev_data[NO_OF_DEVICES];
};


struct driver_private_data drv_data =
{
	.total = NO_OF_DEVICES,
	.dev_data=
	{
		[0] = {.buffer = device1_buffer, .size = DEV_MEM_SIZE_DEV1, .perm = RDONLY, .serial_number = "dev1_serial_number"},
		[1] = {.buffer = device2_buffer, .size = DEV_MEM_SIZE_DEV2, .perm = WRONLY, .serial_number = "dev2_serial_number"},
		[2] = {.buffer = device3_buffer, .size = DEV_MEM_SIZE_DEV3, .perm = RDWR,   .serial_number = "dev3_serial_number"},
		[3] = {.buffer = device3_buffer, .size = DEV_MEM_SIZE_DEV4, .perm = RDWR,   .serial_number = "dev4_serial_number"}
	}
};

static int check_permission(int perm, int access_mode)
{
	if(perm == RDWR)
		return 0;

	else if( (perm == RDONLY) && (RDONLY & access_mode ) && !(WRONLY & access_mode) )
		return 0;

	else if( (perm == WRONLY) && (WRONLY & access_mode)  && !(RDONLY & access_mode) )
		return 0;

	return -EPERM;
}


static int  my_open(struct inode *inode , struct file *filp)
{
	int ret;
	/*get data of device*/
	struct device_private_data *dev_data = container_of(inode->i_cdev, struct device_private_data, cdev);

	pr_info("device access: %d \n",MINOR(inode->i_rdev));
	pr_info("device access mode : %x \n",filp->f_mode);

	//save device data
	filp->private_data = (void*)dev_data;

	/*check permission*/
	ret = check_permission(dev_data->perm , filp->f_mode);
	(!ret)? pr_info("the device is opened successfully\n") : pr_info("permission denied !! \n");


	return ret;
}

static int my_release(struct inode *inode , struct file *filp)
{
	pr_info("the file released !! \n");
	return 0;
}

static ssize_t  my_read(struct file *filp, __user char* buf , size_t count, loff_t *f_pos)
{
	/*get data saved from open method*/
	struct device_private_data *dev_data = (struct device_private_data*)filp->private_data;

	/*check memory*/
	if( (dev_data->size - *f_pos) < count)
		count = dev_data->size - *f_pos;

	/*copy kernel buffer to user buffer*/
	if(copy_to_user(buf,dev_data->buffer,count))
		return -EFAULT;

	/*update file pointer*/
	*f_pos += count;

	return count;

}


static ssize_t my_write(struct file *filp , const  char __user  *buf, size_t count, loff_t *f_pos)
{
	/*get data saved from open method*/
	struct device_private_data *dev_data = (struct device_private_data*)filp->private_data;
	memset(dev_data->buffer,0,dev_data->size);

	/*check memory*/
	if( (dev_data->size - *f_pos) < count )
		count = dev_data->size - *f_pos;

	if(!count)
	{
		pr_err("no space left on the device !!");
		return -ENOMEM;
	}

	/*copy data from user buffer to kernel buffer*/
	if( copy_from_user(dev_data->buffer, buf, count) )
		return -EFAULT;

	/*update file pointer*/
	*f_pos += count;

	return count;
}


static loff_t my_llseek(struct file *filp, loff_t offset, int whence)
{
	/*get data saved from open method */
	struct device_private_data *dev_data = (struct device_private_data*)filp->private_data;

	/*offset have to larger than 0 and smaller than size of device buffer*/
	if( offset > dev_data->size || offset < 0 )
		return -EINVAL;

	else
	{
		if(whence == SEEK_SET)
			filp->f_pos = offset;

		else if(whence == SEEK_CUR)
			filp->f_pos += offset;

		else if(whence == SEEK_END)
			dev_data->size+=offset;

		else 
			return -EINVAL;
	}

	return filp->f_pos;
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



static int __init char_driver_init(void)
{
	int ret , i=0;

	ret = alloc_chrdev_region(&drv_data.dev_number,0,NO_OF_DEVICES,"my_char_driver");
	if(ret < 0)
	{
		pr_err("allocate device number failed !! ");
		goto out;
	}

	
	drv_data.drv_class = class_create(THIS_MODULE,"my_class");
	if(IS_ERR(drv_data.drv_class))
	{
		pr_err("create class device failed !! ");
		ret = PTR_ERR(drv_data.drv_class);
		goto unregister_dev_num;
	}

	for(; i<NO_OF_DEVICES;i++)
	{
		cdev_init(&drv_data.dev_data[i].cdev,&fops);

		drv_data.dev_data[i].cdev.owner = THIS_MODULE;
		ret = cdev_add(&drv_data.dev_data[i].cdev,drv_data.dev_number+i,1);
		if(ret)
		{
			pr_err("register device failed !! \n")	;
			goto class_del;
		}

	

		drv_data.drv_device = device_create(drv_data.drv_class,NULL,drv_data.dev_number+i,NULL,"my_device-%d",i);
		if(IS_ERR(drv_data.drv_device))
		{
			pr_err("create device file failed !! ");
			ret = PTR_ERR(drv_data.drv_device);
			goto class_del;
		}
	}

	pr_info("module insertion successfully !!");
	return 0;

class_del:
	for(;i>=0;i--)
	{
		cdev_del(&drv_data.dev_data[i].cdev);
		device_destroy(drv_data.drv_class,1);
	}
	class_destroy(drv_data.drv_class);

unregister_dev_num:
	unregister_chrdev_region(drv_data.dev_number,NO_OF_DEVICES);

out:
	pr_info("module insertion failed !!");
	return ret;


}


static void __exit char_driver_exit(void)
{
	int i;
	for(i = 0 ;i<NO_OF_DEVICES;i++)
	{
		device_destroy(drv_data.drv_class,drv_data.dev_number+i);
		cdev_del(&drv_data.dev_data[i].cdev);
	}

	class_destroy(drv_data.drv_class);
	unregister_chrdev_region(drv_data.dev_number,NO_OF_DEVICES);
	pr_info("module unloaded !! ");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Duy My");



