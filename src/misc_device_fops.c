#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/fs.h>

#include "drivers_and_interrupts.h"


// 17:06:43 "d" (0x64) PRESSED
//
size_t calc_node_size(struct linked_list_node *node) {
    size_t size = 14;
    if (node->key.state == PRESSED) {
        size += 7;
    } else {
        size += 8;
    }
    if (node->key.ascii_value) {
        size += 6;
    }
    size += strlen(node->key.key_name);
    node->size = size;
    return size;
}


ssize_t read_keyboard_misc_device(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	return simple_read_from_buffer(buf, count, ppos, "jrathelo", strlen("jrathelo"));
}