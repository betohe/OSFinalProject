#include <linux/kernel.h>

 asmlinkage long sys_led(void)
{
		extern ssize_t device_write(struct file *, const char *, size_t, loff_t *);
        printk(“Hello world\n”);
        return 0;
 } 