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
	{ 0x92b57248, __VMLINUX_SYMBOL_STR(flush_work) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x725a561a, __VMLINUX_SYMBOL_STR(fs_bio_set) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf868029a, __VMLINUX_SYMBOL_STR(bio_alloc_bioset) },
	{ 0x76ebea8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0x15692c87, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x20af225c, __VMLINUX_SYMBOL_STR(dm_get_device) },
	{ 0xcc37f55c, __VMLINUX_SYMBOL_STR(blk_limits_io_opt) },
	{ 0x6d0f1f89, __VMLINUX_SYMBOL_STR(dm_table_get_mode) },
	{ 0x20000329, __VMLINUX_SYMBOL_STR(simple_strtoul) },
	{ 0x61c1ef73, __VMLINUX_SYMBOL_STR(dm_register_target) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x27f65ab6, __VMLINUX_SYMBOL_STR(__alloc_pages_nodemask) },
	{ 0x754dac78, __VMLINUX_SYMBOL_STR(blk_limits_io_min) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xad84bef8, __VMLINUX_SYMBOL_STR(dm_table_event) },
	{ 0x2d4232ce, __VMLINUX_SYMBOL_STR(dm_set_target_max_io_len) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1309d060, __VMLINUX_SYMBOL_STR(dm_unregister_target) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
	{ 0x6c2e3320, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x5d69d09a, __VMLINUX_SYMBOL_STR(contig_page_data) },
	{ 0xb79175d4, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0x1f8507e7, __VMLINUX_SYMBOL_STR(bio_put) },
	{ 0xdfe189bd, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x1a5fa4e4, __VMLINUX_SYMBOL_STR(submit_bio) },
	{ 0x26d30461, __VMLINUX_SYMBOL_STR(__free_pages) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x67f7403e, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x4f68e5c9, __VMLINUX_SYMBOL_STR(do_gettimeofday) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xf9e73082, __VMLINUX_SYMBOL_STR(scnprintf) },
	{ 0xb75611c1, __VMLINUX_SYMBOL_STR(dm_put_device) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb02c044e, __VMLINUX_SYMBOL_STR(try_module_get) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A0BF723A8626A3ED56F1904");
