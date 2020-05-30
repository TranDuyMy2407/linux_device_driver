
obj-m = bai_tap.o
bai_tap-objs = main.o dem_san_pham.o 

KDIR=/lib/modules/$(shell uname -r)/build M=$(PWD)

all:
	make -C $(KDIR) modules

clean:
	make -C $(KDIR) clean
insert:
	sudo insmod bai_tap.ko
remove:
	sudo rmmod bai_tap
