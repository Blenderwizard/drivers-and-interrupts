#ifndef DRIVERS_AND_INTERRUPTS_H
#define DRIVERS_AND_INTERRUPTS_H

#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/interrupt.h>

ssize_t read_keyboard_misc_device(struct file *file, char __user *buf,
				  size_t count, loff_t *ppos);
irqreturn_t key_logger_isr(int i, void *dummy);

enum state {
	PRESSED = true,
	RELEASED = false,
};

struct KeyboardCaptureData {
	uint64_t code;
	char *key_name;
	enum state state;
	char ascii_value;
};

struct linked_list_node {
	struct KeyboardCaptureData key;
	struct timespec64 when;
	size_t size;
	struct list_head list;
};

struct logger {
	char shifted;
	char capslocked;
};

#ifndef EXTERN
#define EXTERN extern
#endif
extern struct list_head keyboard_log;
extern struct mutex lock;

void print_keyboard_line(struct KeyboardCaptureData data);
size_t calc_node_size(struct linked_list_node *node);

#endif
