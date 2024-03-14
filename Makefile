KDIR ?= /lib/modules/`uname -r`/build
SRC = src/main.c src/misc_device_fops.c src/keyboard_logger.c

all: driver_and_interrupts.ko

driver_and_interrupts.ko: $(SRC)
	$(MAKE) -C $(KDIR) M=$$PWD LLVM=1

install: modprobe rules_install

modules_install: driver_and_interrupts.ko
	$(MAKE) -C $(KDIR) M=$$PWD LLVM=1 modules_install

rules_install: modules_install 99-Hotload-Keyboard.rules driver_and_interrupts.ko
	cp -v 99-Hotload-Keyboard.rules /etc/udev/rules.d/
	udevadm control --reload-rules
	udevadm trigger

clean:
	$(MAKE) -C $(KDIR) M=$$PWD LLVM=1 clean

modprobe: modules_install
	modprobe driver_and_interrupts

rmmod:
	rmmod driver_and_interrupts

.PHONY: all clean modules_install rmmod modprobe install rules_install