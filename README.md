# Drivers_and_interrupts

> **Warning**
>
> Use of this project is for **Educational / Testing purposes only**. Using it on **unauthorised machines** is **strictly forbidden**. If somebody is found to use it for **illegal / malicious intent**, author of the repo will **not** be held responsible.


Drivers and Interrupts is a 42 Linux Kernel Module Project written in C where you have to code a Linux Kernel keylogger.

This keylogger is a Linux Kernel module and it must be able to capture a key being pressed and released. The module must then create a misc device character file, which when read prints out in a log the keys that it captured. The logs are also saved the the kernel logs.


On exit, the module will print the number of keys captured.

To build, install and run the module:
```sh
make install
```

To build, install and run the module when a keyboard is plugged in:
```sh
make rule_install
```
> **Note**
>
> Only compatable on systems where udev is installed.

> **Note**
>
> This Module has only been tested on Linux Kernel version 6.8.0. It should be compatable with most other versions.

> **Note**
>
> The Makefile was configured for Linux Kernels that are compiled using the LLVM toolkit.
