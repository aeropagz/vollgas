#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>

#include "control.h"
#include "gpio.h"

#define DEV_NAME "VOLLGAS"

static dev_t myDevNumber;
static struct cdev *myCdev;
static struct class *myClass;
static struct device *myDevice;

static unsigned int command = DEFAULT_WORD;

struct OutputBuffer out;

static struct hrtimer mytimer;
ktime_t mytime;

static enum hrtimer_restart writePayload(struct hrtimer *hrt)
{
    if (out.position >= out.length)
        return HRTIMER_NORESTART;

    unsigned char value = readBitFromBuffer(out.encodedBits, out.position++);
    printk("bit: %d | val: %d ", out.position - 1, value);
    if(value)
    hrtimer_add_expires_ns(&mytimer, 58000);
    return HRTIMER_RESTART;
}

void sendWord(unsigned int word)
{
    encodePayload(&out, word);
    mytimer.function = writePayload;
    mytime = ktime_set(0, 58000);
    hrtimer_start(&mytimer, mytime, HRTIMER_MODE_REL);
}

void send_left_fast(void)
{
    printk("start\n");
    setDirection(&command, 1);
    setMotor(&command, 1);
    setSpeed(&command, 255);
    sendWord(command);
    command = DEFAULT_WORD;
    printk("Command left fast sending...");
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
    printk("INIT vollgas\n");
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
    printk("EXIT vollgas\n");
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
