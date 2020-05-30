#include<linux/module.h>
#include<linux/platform_device.h>

static int my_probe(struct platform_device *pdev)
{
	pr_info("hello world !! \n");
	return 0;
}

static int my_remove(struct platform_device *pdev)
{
	pr_info("good byee !!! \n");
	return 0;
}

static struct platform_driver my_pdriver = 
{
	.probe = my_probe,
	.remove = my_remove,
	.driver = 
	{
		.name = KBUILD_MODNAME,
		.owner = THIS_MODULE
	}

};

static int __init pdriver_init(void)
{
	platform_driver_register(&my_pdriver);
	return 0;
}

static void __exit pdriver_exit(void)
{
	platform_driver_unregister(&my_pdriver);
}

module_init(pdriver_init);
module_exit(pdriver_exit);
MODULE_LICENSE("GPL");
