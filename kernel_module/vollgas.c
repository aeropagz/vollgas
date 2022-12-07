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
#define WR_VALUE _IOW('a','a',struct myType*)
#define RD_VALUE _IOR('a','b',struct myType*)	 	

static dev_t myDevNumber;
static struct cdev *myCdev;
static struct class *myClass;
static struct device *myDevice;

static unsigned int command = DEFAULT_WORD;
struct OutputBuffer out;

struct Command {
    uint32_t motor1Speed;
    uint32_t motor2Speed;
    uint8_t motor1Direction;
    uint8_t motor2Direction;
} data;

static struct hrtimer mytimer;
ktime_t mytime;

static enum hrtimer_restart writePayload(struct hrtimer *hrt)
{
    if (out.position >= out.length)
        return HRTIMER_NORESTART;

    unsigned char value = readBitFromBuffer(out.encodedBits, out.position--);
    // printk("bit: %u | val: %d ", out.position + 1, value);
    if (value)
    {
        set_high();
    }
    else
    {
        set_low();
    }

    hrtimer_add_expires_ns(&mytimer, 58000);
    return HRTIMER_RESTART;
}

void sendWord(unsigned int word)
{
    encodePayload(&out, word);
    out.position = (out.length - 1);
    printk("Starting at pos: %u", out.position);
    mytimer.function = writePayload;
    mytime = ktime_set(0, 58000);
    hrtimer_start(&mytimer, mytime, HRTIMER_MODE_REL);
}

void send_left_fast(void)
{
    printk("Start building command: LEFT_FAST_M1\n");
    setDirection(&command, 1);
    setMotor(&command, 1);
    setSpeed(&command, 100);
    printk("Command: %u", command);
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

static long ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int i = 0;
    printk("ioctrl called!\n");
    switch (cmd)
    {
    case WR_VALUE:
        printk("Receiving data");
        if (copy_from_user(&data, (struct Command *)arg, sizeof(data)))
        {
            pr_err("Error receiving\n");
        }


        break;
    case RD_VALUE:
        printk("Sending data");
        if (copy_to_user((struct myType *)arg, &data, sizeof(struct Command)))
        {
            pr_err("Error sending\n");
        }
        break;
    default:
        pr_info("No Comand recognized\n");
        break;
    }
    return 0;
}

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
