#include"platform_device.h"


void platform_device_release(struct device *dev)
{
}

struct platform_device_data pdata[4] = 
{
	[0] = {.size = 512 , .perm = RDWR, .serial_number = "device0"},
	[1] = {.size = 512 , .perm = RDWR, .serial_number = "device1"},
	[2] = {.size = 512 , .perm = RDONLY, .serial_number = "device2"},
	[3] = {.size = 512 , .perm = WRONLY, .serial_number = "device3"},

};

struct platform_device platform_dev0=
{
	.name = "my_platform_device-0",
	.id = 0,
	.dev = {.platform_data = &pdata[0] , .release = platform_device_release}
	
};

struct platform_device platform_dev1=
{
	.name = "my_platform_device-1",
	.id = 1,
	.dev = {.platform_data = &pdata[1] , .release = platform_device_release}

};


struct platform_device platform_dev2=
{
	.name = "my_platform_device-2",
	.id = 2,
	.dev = {.platform_data = &pdata[2] , .release = platform_device_release}
	
};


struct platform_device platform_dev3=
{
	.name = "my_platform_device-3",
	.id = 3,
	.dev = {.platform_data = &pdata[3] , .release = platform_device_release}
	
};


static int __init platform_device_init(void)
{
	platform_device_register(&platform_dev0);
	platform_device_register(&platform_dev1);
	platform_device_register(&platform_dev2);
	platform_device_register(&platform_dev3);


	return 0;
}

static void __exit platform_device_exit(void)
{
	platform_device_unregister(&platform_dev0);
	platform_device_unregister(&platform_dev1);
	platform_device_unregister(&platform_dev2);
	platform_device_unregister(&platform_dev3);
	pr_info("module unloaded !! \n");


}

module_init(platform_device_init);
module_exit(platform_device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Duy My");




