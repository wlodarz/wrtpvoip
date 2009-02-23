
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/ar7/ar7.h>
#include <asm/ar7/gpio.h>

#define DRV_VERSION "0.0.1"
#define DRV_DESC "testapp driver"


static int __init testapp_init_module(void) {
	printk(KERN_ERR "test app loaded\n");
	
	// init HAL
	
	// init voicepath api
	
	// perform some tests
	
	return 0;
}

static void __exit testapp_cleanup_module(void)
{
	// clean up test app
	
	// clean up voicepath api
	
	// clean up HAL
	
	printk(KERN_ERR "test app unloaded\n");
}

module_init(testapp_init_module);
module_exit(testapp_cleanup_module);

