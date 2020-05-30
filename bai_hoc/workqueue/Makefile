obj-m = work_queue.o

KDIR = /lib/modules/$(shell uname -r)/build M=$(PWD)

all:
	make -C $(KDIR) modules
clean:
	make -C $(KDIR) clean

insert:
	sudo insmod work_queue.ko
remove:
	sudo rmmod work_queue
