#include <linux/kernel.h>

 asmlinkage long sys_led(void)
{

        printk(“Hello world\n”);
        return 0;
 } 