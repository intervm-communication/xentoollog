export KDIR ?= /lib/modules/`uname -r`/build

# Compiler Flags
ccflags-y := -Wall -Werror -I$(M)/../include -I$(M)/../include/linux_kernel

# Build the OpenXT framebuffer module.
obj-m += xentoollog.o
obj-m += xentoollog_test.o
xentoollog-objs := xtl_core.o xtl_logger_printk.o

all: modules

modules:
	$(MAKE) -C $(KDIR) M=$(shell pwd)

modules_install: 
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules_install
