#include <linux/time.h>
#include <linux/gpio.h>

#define PIN_NUM 17
#define MY_GPIO_INT_NAME "my_pin_int"

int init_gpio(void)
{
    printk("Init Gpio\n");
    if (gpio_request(PIN_NUM, "myPin") < 0)
        return -1;
    if (gpio_direction_output(PIN_NUM, 0))
    {
        printk("direction_output failed");
        gpio_free(PIN_NUM);
        return -1;
    }
    return 0;
}

void set_high(void)
{
    gpio_set_value(PIN_NUM, 1);
}

void set_low(void)
{
    gpio_set_value(PIN_NUM, 0);
}

void clean_up_gpio(void)
{
    gpio_free(PIN_NUM);
}