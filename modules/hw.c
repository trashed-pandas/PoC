#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World module");
MODULE_VERSION("0.1");

static int __init hello_init(void) {
    printk(KERN_INFO "hello, world\n");
    return 0;  // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void) {
    printk(KERN_INFO "goodbye, world\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
