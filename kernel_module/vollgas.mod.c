#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x826b8746, "module_layout" },
	{ 0x695bf5e9, "hrtimer_cancel" },
	{ 0x5105d7f, "cdev_del" },
	{ 0xb33af203, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xa54133aa, "kobject_put" },
	{ 0x5e657d97, "class_destroy" },
	{ 0xa362bf8f, "hrtimer_init" },
	{ 0x8360e9, "device_create" },
	{ 0x93c8fa59, "__class_create" },
	{ 0x8e6ad741, "cdev_add" },
	{ 0x6453db1a, "cdev_alloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x5f754e5a, "memset" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xec523f88, "hrtimer_start_range_ns" },
	{ 0xf0ef52e8, "down" },
	{ 0xfe990052, "gpio_free" },
	{ 0xb77942ad, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x92997ed8, "_printk" },
	{ 0x581cde4e, "up" },
	{ 0xa339085b, "gpiod_set_raw_value" },
	{ 0xf7d5fff7, "gpio_to_desc" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3D3D4240036EB147C46ABCE");
