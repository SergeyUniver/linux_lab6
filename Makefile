NAME=remainer
obj-m += $(NAME).o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
check:
	cat /proc/modules | grep $(NAME)
install:
	sudo insmod ./$(NAME).ko
remove:
	sudo rmmod $(NAME)
info:
	modinfo $(NAME).ko
show:
	dmesg
