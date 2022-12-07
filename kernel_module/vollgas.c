#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/semaphore.h>

#include "../commons/data.h"
#include "control.h"
#include "gpio.h"

#define DEV_NAME "VOLLGAS"
#define WR_VALUE _IOW('a','a',struct myType*)
#define RD_VALUE _IOR('a','b',struct myType*)	 	

static dev_t myDevNumber;
static struct cdev *myCdev;
static struct class *myClass;
static struct device *myDevice;
static struct semaphore my_semaphore;


static MotorData motorControl;
struct OutputBuffer out;

static struct hrtimer mytimer;
ktime_t mytime;

static enum hrtimer_restart writePayload(struct hrtimer *hrt)
{
    if (out.position >= out.length){
        set_high();
        up(&my_semaphore);
        printk("released semaphore\n\n\n");
        return HRTIMER_NORESTART;
    }

    unsigned char value = readBitFromBuffer(out.encodedBits, out.position--);
    // printk("bit: %u | val: %d \n", out.position + 1, value);
    if (value) {
        set_high();
    }
    else {
        set_low();
    }

    hrtimer_add_expires_ns(&mytimer, 58000);
    return HRTIMER_RESTART;
}

void sendWord(uint32_t word)
{
    printk("Wait for semaphore\n");
    down(&my_semaphore);
    printk("Got semaphore\n");
    encodePayload(&out, word);
    out.position = (out.length - 1);
    printk("Starting at pos: %u", out.position);
    mytimer.function = writePayload;
    mytime = ktime_set(0, 58000);
    hrtimer_start(&mytimer, mytime, HRTIMER_MODE_REL);
}

void send_left_fast(void)
{
    uint32_t command = DEFAULT_WORD;
    printk("Start building command: LEFT_FAST_M1\n");
    setDirection(&command, 1);
    setMotor(&command, 1);
    setSpeed(&command,100);
    printk("Command: %u", command);
    sendWord(command);
    command = DEFAULT_WORD;
    printk("Command left fast sending...");
}


uint32_t buildCommandMotor(MotorData* newData){
    uint32_t commandWord = DEFAULT_WORD;
    setMotor(&commandWord, newData->id);
    setDirection(&commandWord, (newData->direction));
    setSpeed(&commandWord, newData->speed);
    printk("Command: %u\n", commandWord);
    return commandWord;
}

static long ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("ioctrl called!\n");
    switch (cmd)
    {
    case WR_VALUE:
        printk("Receiving data");
        if (copy_from_user(&motorControl, (struct MotorData *)arg, sizeof(motorControl)))
        {
            pr_err("Error receiving\n");
        }
        printk("id: %u, speed: %u, direction: %u\n", motorControl.id, motorControl.speed, motorControl.direction);
        uint32_t command = buildCommandMotor(&motorControl);
        sendWord(command);
        break;

    case RD_VALUE:
        printk("Sending data");
        if (copy_to_user((struct MotorData *)arg, &motorControl, sizeof(motorControl)))
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

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = NULL,
    .open = NULL,
    .release = NULL,
    .unlocked_ioctl = ioctl,
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
    sema_init(&my_semaphore, 1);

    // send_left_fast();

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
