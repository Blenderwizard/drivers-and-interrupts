#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/io.h>

#include "drivers_and_interrupts.h"

#define KEYBOARD_IRQ 1

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .read           = read_keyboard_misc_device,
};

static struct miscdevice device = {
    .fops           = &fops,
    .name           = "drivers_and_interrupts",
    .minor          = MISC_DYNAMIC_MINOR,
    .mode           = 0444,
};

struct mutex lock = __MUTEX_INITIALIZER(lock);
static struct logger key_logger =  { 0, 0 };

static int __init m_init(void) {
    struct linked_list_node *ptr, *tmp;
    int err = 0;
	printk(KERN_INFO "Drivers and Interrupts (init)\n");

    if (request_irq(1, key_logger_isr, IRQF_SHARED, "i8042", &key_logger)) {
        err = -EBUSY;
        goto err_free_linked_list;
    }
    if (misc_register(&device)) {
        err = -ENOMEM;
        goto err_free_keyboard_notifier;
	}
    mutex_init(&lock);
	return 0;

err_free_keyboard_notifier:
    free_irq(1, &key_logger);
err_free_linked_list:
    list_for_each_entry_safe(ptr, tmp, &keyboard_log, list) {
        list_del(&ptr->list);
        kfree(ptr);
    }
    return err;
}

static void __exit m_exit(void) {
    struct linked_list_node *ptr, *tmp;

    misc_deregister(&device);
    list_for_each_entry_safe(ptr, tmp, &keyboard_log, list){
        list_del(&ptr->list);
        kfree(ptr);
    }
    free_irq(1,&key_logger);
    mutex_destroy(&lock);
	printk(KERN_INFO "Drivers and Interrupts (exit)\n");
}

module_init(m_init);
module_exit(m_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jrathelo");
MODULE_DESCRIPTION("Drivers-and-Interrupts");
MODULE_VERSION("1");
