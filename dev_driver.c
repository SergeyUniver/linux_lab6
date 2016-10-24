#include <linux/kernel.h> /* Для printk() и т.д. */
#include <linux/module.h> /* Эта частичка древней магии, которая оживляет модули */
#include <linux/init.h> /* Определения макросов */
#include <linux/fs.h>
#include <asm/uaccess.h> /* put_user */
#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>



// Ниже мы задаём информацию о модуле, которую можно будет увидеть с помощью Modinfo
MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Matvei Nazaruk" );
MODULE_DESCRIPTION( "Test module" );
MODULE_SUPPORTED_DEVICE( "timer_out" );

#define SUCCESS 0
MODULE_SUPPORTED_DEVICE( "test" ); 

#define SUCCESS 0
#define DEVICE_NAME "test"


static enum hrtimer_restart timer_function (struct hrtimer * unused)
{

    if (tick != 0) {
        printk( "%s\n", text);
        kt_periode = ktime_set(tick, 0); 
    }
    hrtimer_forward_now(& htimer, kt_periode);
    return HRTIMER_RESTART;
}

static void timer_init (void)
{
    kt_periode = ktime_set(1, 0); 
    hrtimer_init (& htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    htimer.function = timer_function;
    hrtimer_start(& htimer, kt_periode, HRTIMER_MODE_REL);
}


static int __init test_init (void)
{
    printk( KERN_ALERT "TEST driver loaded!\n" );
    timer_init();
	hrtimer_cancel(& htimer);
    major_number = register_chrdev( 0, DEVICE_NAME, &fops );
    if ( major_number < 0 ) {
        printk( "Registering the character device failed with %d\n", major_number );
        return major_number;
    }
    printk( "Please, create a dev file with 'mknod /dev/test c %d 0'.\n", major_number );
    return SUCCESS;
}



// Указываем наши функции загрузки и выгрузки
module_init( test_init );
module_exit( test_exit );
