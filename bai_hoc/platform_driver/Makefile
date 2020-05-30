obj-m = platform_driver.o

KDIR = /lib/modules/$(shell uname -r)/build M=$(PWD)

all:
	make -C $(KDIR) modules
clean:
	make -C $(KDIR) clean
insert:
	sudo insmod platform_driver.ko
remove:
	sudo rmmod platform_driver
