#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>

#include "control.h"
#include "gpio.h"

#define DEV_NAME "VOLLGAS"

static dev_t myDevNumber;
static struct cdev *myCdev;
static struct class *myClass;
static struct device *myDevice;

static int waiting = 0;
static int position = 0;

static unsigned int command = DEFAULT_WORD;

static struct hrtimer mytimer;
ktime_t mytime;

static enum hrtimer_restart hrtimer_callback(struct hrtimer *hrt)
{
    hrtimer_add_expires_ns(&mytimer, 1000000000);
    return HRTIMER_RESTART;
}

static enum hrtimer_restart one_set_low(struct hrtimer *hrt)
{
    set_low();
    mytimer.function = finish;
    hrtimer_add_expires_ns(&mytimer, 58000);
    return HRTIMER_RESTART;
}

static enum hrtimer_restart zero_set_low(struct hrtimer *hrt)
{
    set_low();
    mytimer.function = finish;
    hrtimer_add_expires_ns(&mytimer, 116000);
    return HRTIMER_RESTART;
}

static enum hrtimer_restart finish(struct hrtimer *hrt)
{
    waiting = 0;
    return HRTIMER_NORESTART;
}


void sendWord(unsigned int word)
{
    if (sizeof word != 32)
    {
        printk("word has wrong size!");
        return;
    }

    while (position < 32 && !waiting)
    {
        if (word & (1 << position))
        {
            // is one, send one
            send_one();
            printk("1");
        }
        else
        {
            // is zero, send zero
            send_zero();
            printk("0");
        }
        position++;
    }
    position = 0;
}

void send_one()
{
    mytimer.function = one_set_low;
    mytime = ktime_set(0, 58000);
    waiting = 1;
    hrtimer_start(&mytimer, mytime, HRTIMER_MODE_REL);
    set_high();
}

void send_zero() {
    mytimer.function = one_set_low;
    mytime = ktime_set(0, 116000);
    waiting = 1;
    hrtimer_start(&mytimer, mytime, HRTIMER_MODE_REL);
    set_high();
}

void send_left_fast(){
    setDirection(&command, 1);
    setMotor(&command, 1);
    setSpeed(&command, 255);
    printk("command: %d", command);
    sendWord(command);
    command = DEFAULT_WORD;
}

static ssize_t my_drv_read(struct file *instance, char __user *user, size_t count, loff_t *offset)
{
    unsigned long not_copied, to_copy;
    to_copy = min(count, sizeof(hits_per_second));
    not_copied = copy_to_user(user, &hits_per_second, to_copy);
    *offset += to_copy - not_copied;
    printk("Habe %ld Bytes geschrieben\n", to_copy - not_copied);
    printk("Hits: %ld", hits_per_second);
    return to_copy - not_copied;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = NULL,
    .open = NULL,
    .release = NULL,
};

/* Wenn Modul in den Kernel geladen wird */
static int __init mod_init(void)
{
    printk("init wird ausgef?hrt\n");
    if (alloc_chrdev_region(&myDevNumber, 0, 1, DEV_NAME) < 0)
        return EIO;

    myCdev = cdev_alloc();

    if (myCdev == NULL)
        goto free_device_number;

    myCdev->owner = THIS_MODULE;
    myCdev->ops = &fops;

    if (cdev_add(myCdev, myDevNumber, 1))
        goto free_cdev;

    myClass = class_create(THIS_MODULE, DEV_NAME);
    if (IS_ERR(myClass))
    {
        pr_err("Keine Anmeldung beim Udev\n");
        goto free_cdev;
    }

    myDevice = device_create(myClass, NULL, myDevNumber, NULL, "%s", DEV_NAME);

    if (IS_ERR(myDevice))
    {
        pr_err("create_device failed\n");
        goto free_class;
    }

    // gpio setup
    if (init_gpio() != 0)
    {
        printk("Error init gpio!\n");
        goto free_class;
    };

    // timer setup
    hrtimer_init(&mytimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    send_left_fast();
    
    return 0;

free_class:
    class_destroy(myClass);

free_cdev:
    kobject_put(&myCdev->kobj);

free_device_number:
    unregister_chrdev_region(myDevNumber, 1);
    return -EIO;
}

/* If module is removed from kernel */
static void __exit mod_exit(void)
{
    printk("__exit wird ausgef?hrt\n");
    clean_up_gpio();
    device_destroy(myClass, myDevNumber);
    class_destroy(myClass);
    cdev_del(myCdev);
    unregister_chrdev_region(myDevNumber, 1);

    hrtimer_cancel(&mytimer);

    return;
}

module_init(mod_init);
module_exit(mod_exit);

/* Metainformation */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Klaas Pelzer");
MODULE_DESCRIPTION("Beispiel BK110-2");
