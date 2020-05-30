#include<linux/module.h>
#include<linux/usb.h>

static int pen_probe(struct usb_interface *interface , const struct usb_device_id *id)
{
	pr_info("pen drive <%04x:%04x>",id->idVendor,id->idProduct);
	pr_info("the usb has probed !!! \n");
	return 0;
}

static void pen_disconnect(struct usb_interface *interface)
{
	pr_info("the usb has been remove !!! \n");
}


static struct usb_device_id pen_table[]=
{
	{USB_DEVICE( 0x0930 , 0x6544)},
	{USB_DEVICE(0x0B05 , 0x7780)},
	{}	
};


static struct usb_driver pen_driver = 
{
	.name = "usb_driver",
	.probe = pen_probe,
	.disconnect = pen_disconnect,
	.id_table = pen_table
	
};


static int __init usb_driver_init(void)
{
	return usb_register(&pen_driver);
}

static void __exit usb_driver_exit(void)
{
	usb_deregister(&pen_driver);
}


module_init(usb_driver_init);
module_exit(usb_driver_exit);
MODULE_LICENSE("GPL");
