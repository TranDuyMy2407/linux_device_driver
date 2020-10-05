#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x2a51cf90, "module_layout" },
	{ 0xefc93a94, "class_destroy" },
	{ 0x7e4fb0b7, "platform_driver_unregister" },
	{ 0xbf301273, "__platform_driver_register" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x359cafaf, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x5f754e5a, "memset" },
	{ 0xaff3d0c0, "device_destroy" },
	{ 0x7141965f, "cdev_del" },
	{ 0x6ebc8180, "device_create" },
	{ 0x864bcfcc, "cdev_add" },
	{ 0xbd6abf9f, "cdev_init" },
	{ 0xc5850110, "printk" },
	{ 0xf1714917, "devm_kmalloc" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "8A90855887581C93478CBF5");
