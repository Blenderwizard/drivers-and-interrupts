KDIR ?= /lib/modules/`uname -r`/build

all: driver_and_interrupts.ko

driver_and_interrupts.ko: src/lib.rs
	$(MAKE) -C $(KDIR) M=$$PWD LLVM=1

modules_install:
	$(MAKE) -C $(KDIR) M=$$PWD LLVM=1 modules_install

clean:
	$(MAKE) -C $(KDIR) M=$$PWD LLVM=1 clean

insmod:
	insmod driver_and_interrupts.ko

rmmod:
	rmmod driver_and_interrupts

.PHONY: all clean modules_install rmmod insmod