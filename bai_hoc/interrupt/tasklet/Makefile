obj-m = my_tasklet.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

insert:
	sudo insmod my_tasklet.ko
remove:
	sudo rmmod my_tasklet
