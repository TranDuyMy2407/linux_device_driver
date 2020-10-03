#include<linux/module.h>
#include<linux/platform_device.h>
#include"platform_device.h"


void platform_device_release(struct device *dev)
{
}

struct platform_device_data pdata[2] = 
{
	[0] = {.size = 512 , .perm = RDWR, .serial_number = "device1"},
	[1] = {.size = 512 , .perm = RDWR, .serial_number = "device2"}
};

struct platform_device platform_dev1=
{
	.name = "my_platform_device",
	.id = 0,
	.dev = {.platform_data = &pdata[0] , .release = platform_device_release}
	
};

struct platform_device platform_dev2=
{
	.name = "my_platform_device",
	.id = 1,
	.dev = {.platform_data = &pdata[1] , .release = platform_device_release}

};


static int __init platform_device_init(void)
{
	platform_device_register(&platform_dev1);
	platform_device_register(&platform_dev2);


	return 0;
}

static void __exit platform_device_exit(void)
{
	platform_device_unregister(&platform_dev1);
	platform_device_unregister(&platform_dev2);
}

module_init(platform_device_init);
module_exit(platform_device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Duy My");




