#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init hello_init(void){ // na inicializaciji nasega inita klicemo to funkcijo
    printk(KERN_INFO"Hello world\n");  // print kernel
    return 0;
}

static void __exit hello_exit(void){
    printk(KERN_INFO "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin");
MODULE_DESCRIPTION("Demo modul");

// naredimo make file za compiler