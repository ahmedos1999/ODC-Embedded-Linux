#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\x7c\x24\xc7\x40"
	"si_meminfo\0\0"
	"\x18\x00\x00\x00\x38\xcc\x18\x7d"
	"vm_node_stat\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x14\x00\x00\x00\xdf\xbd\x0b\xcd"
	"filp_open\0\0\0"
	"\x14\x00\x00\x00\x1f\x40\x85\xee"
	"kernel_read\0"
	"\x14\x00\x00\x00\x80\x20\x33\x5b"
	"filp_close\0\0"
	"\x10\x00\x00\x00\xe6\x6e\xab\xbc"
	"sscanf\0\0"
	"\x28\x00\x00\x00\xb3\x1c\xa2\x87"
	"__ubsan_handle_out_of_bounds\0\0\0\0"
	"\x14\x00\x00\x00\x52\x4d\x35\x6a"
	"proc_remove\0"
	"\x14\x00\x00\x00\x16\x7e\x9b\x2f"
	"seq_printf\0\0"
	"\x14\x00\x00\x00\xa2\x41\xf2\xdd"
	"init_task\0\0\0"
	"\x2c\x00\x00\x00\xc6\xfa\xb1\x54"
	"__ubsan_handle_load_invalid_value\0\0\0"
	"\x14\x00\x00\x00\x97\xc5\xcc\x79"
	"seq_read\0\0\0\0"
	"\x14\x00\x00\x00\xea\x31\xa2\x35"
	"seq_lseek\0\0\0"
	"\x18\x00\x00\x00\x10\xb2\xc9\xf3"
	"single_release\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x14\x00\x00\x00\xc1\xb2\x4a\x4f"
	"proc_create\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\x1f\xd8\xa1\x3c"
	"single_open\0"
	"\x18\x00\x00\x00\xd7\xd3\x75\x6d"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "486FFAAE5B08ECB11151F39");
