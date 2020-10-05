obj-m = platform_driver_dt.o

KDIR = /lib/modules/$(shell uname -r)/build M=$(PWD)

all:
	make -C $(KDIR) modules
clean:
	make -C $(KDIR) clean
insert:
	sudo insmod platform_driver_dt.ko
remove:
	sudo rmmod platform_driver_dt
