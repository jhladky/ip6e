#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init ip6e_init(void) {

   printk(KERN_INFO "Hello.\n");
   return 0; //non-0 return means the module failed
}

static void __exit ip6e_exit(void) {
   printk(KERN_INFO "Goodbye.\n");
}

module_init(ip6e_init);
module_exit(ip6e_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jacob Hladky");
MODULE_DESCRIPTION("Test driver for ip6e project.");

