obj-m=buf.o
buf-y=main.o ring/ring_api.o test/ring.o

CURRENT_PATH=`pwd`
KERNEL=`uname -r`
KERNEL_PATH=/usr/src/kernels/$(KERNEL)

all:
	make -C $(KERNEL_PATH) EXTRA_CFLAGS=-I$(CURRENT_PATH)/include M=$(CURRENT_PATH) modules
clean:
	make -C $(KERNEL_PATH) EXTRA_CFLAGS=-I$(CURRENT_PATH)/include M=$(CURRENT_PATH) clean 

