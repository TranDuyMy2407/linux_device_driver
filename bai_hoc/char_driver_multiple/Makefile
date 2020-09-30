obj-m := char_driver.o

KDIR = /lib/modules/$(shell uname -r)/build M=$(PWD)

all:
	make -C $(KDIR) modules

clean:
	make -C $(KDIR) clean

help:
	make -C $(KDIR) help

insert:
	sudo insmod char_driver.ko
	sudo chmod 777 /dev/my_device-1
	sudo chmod 777 /dev/my_device-2
	sudo chmod 777 /dev/my_device-3
	sudo chmod 777 /dev/my_device-4


remove:
	sudo rmmod char_driver
