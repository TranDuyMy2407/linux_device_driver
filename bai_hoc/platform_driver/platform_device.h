#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>

//access mode

#define RDONLY	0x1
#define WRONLY	0x2
#define RDWR	0x3

/*platform device data*/
struct platform_device_data
{
	unsigned int size;
	unsigned int perm;
	const char *serial_number;


};


/*Device private data structure*/
struct device_private_data
{
	struct platform_device_data pdata;
        char *buffer;
        dev_t  dev_num;
        struct cdev cdev;

};


/*Driver private data*/
struct drv_private_data
{
        int total;
        dev_t device_num_base;
        struct class *drv_class;
        struct device *drv_device;
};

