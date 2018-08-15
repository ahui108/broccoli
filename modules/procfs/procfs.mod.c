#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x2005612d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x4860fe4f, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xfff54354, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xeeb9e940, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0xea40aa27, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0xf1b9f044, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x30fe3b7b, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x457b15f3, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "95099852D2F258CE6320326");
