#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/fs.h>

#include "drivers_and_interrupts.h"

ssize_t read_keyboard_misc_device(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	return simple_read_from_buffer(buf, count, ppos, "jrathelo", strlen("jrathelo"));
}