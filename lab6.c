#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>

static struct kobject *remainer_kobject;
static struct timer_list my_timer;
static int timer_value;
static int time_period = 4;

static void restart_timer(void)
{
	if (timer_value > 0)
	{
		mod_timer( &my_timer, jiffies + msecs_to_jiffies(time_period * 1000) );
		timer_value--;
	}
}

static void my_timer_callback( unsigned long data )
{
	printk(KERN_ERR "Hello\n");
	restart_timer();
}

static ssize_t timer_value_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
	return sprintf(buf, "%d\n", timer_value);
}

static ssize_t timer_value_store(struct kobject *kobj, struct kobj_attribute *attr,
					char *buf, size_t count)
{
	printk(KERN_ERR "Hello from store\n");
	sscanf(buf, "%du", &timer_value);
	restart_timer();
	return count;
}


static struct kobj_attribute timer_value_attribute =__ATTR(timer, 0666, timer_value_show,
													timer_value_store);

static int __init mymodule_init (void)
{
	int error = 0;
	
	printk(KERN_ERR "Module remainer_kobject initialized successfully \n");

	remainer_kobject = kobject_create_and_add("remainer_kobject",
											 kernel_kobj);
	if(!remainer_kobject)
			return -ENOMEM;

	error = sysfs_create_file(remainer_kobject, &timer_value_attribute.attr);
	if (error) {
			printk(KERN_ERR "failed to create the timer file in /sys/kernel/remainer_kobject \n");
	}

	setup_timer( &my_timer, my_timer_callback, 0 );
	
	return error;
}

static void __exit mymodule_exit (void)
{
	int ret;
	printk(KERN_ERR "Module remainer_kobject un initialized successfully \n");
	kobject_put(remainer_kobject);
	ret = del_timer( &my_timer );
	if (ret) 
		printk(KERN_ERR "The timer is still in use...\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mochulsky Sergey");
MODULE_DESCRIPTION("Timer.");
MODULE_VERSION("1.0.0.0");
