obj-m += ip6e.o
# <module_name>-objs := <src1>.o <src2>.o ...

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
