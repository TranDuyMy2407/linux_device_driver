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
	sudo chmod 777 /dev/ | grep my_device

remove:
	sudo rmmod char_driver
