obj-m = wait_queue.o

KDIR = /lib/modules/$(shell uname -r)/build M=$(PWD)

all:
	make -C $(KDIR) modules
clean:
	make -C $(KDIR) clean

insert:
	sudo insmod wait_queue.ko
remove:
	sudo rmmod wait_queue
