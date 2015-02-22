#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x20d60b07, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe924b3, __VMLINUX_SYMBOL_STR(insane_unregister) },
	{ 0x2c08320b, __VMLINUX_SYMBOL_STR(insane_register) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=insane_striping";


MODULE_INFO(srcversion, "B711B5ED60D02DBD49A1501");
