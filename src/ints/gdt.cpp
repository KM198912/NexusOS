#include <ints/gdt.hpp>
#include <core/main.hpp>
#include <lib/string.hpp>

using namespace system::hal::interrupts;
using namespace system::hal::interrupts::gdt;

__cdecl gdt32_register *gdt::__gdtr = nullptr;
__cdecl gdt32_entry *gdt::gdt32_table = nullptr;
__cdecl tss_entry_struct *gdt::tss_sect = nullptr;
__cdecl tss_entry_struct gdt::__tss = {};

void manager_t::set_desc(int n, uint32_t base, uint32_t limit, gdt32_entry_access access, gdt32_entry_flags flags)
{
    gdt32_table[n].base_low = base & 0xFFFF;
	gdt32_table[n].base_middle = (base >> 16) & 0xFF;
	gdt32_table[n].base_high = (base >> 24) & 0xFF;
	
	gdt32_table[n].limit_low = limit & 0xFFFF;
	gdt32_table[n].limit_high = (limit >> 16) & 0x0F;

	gdt32_table[n].granularity = flags.granularity;
	gdt32_table[n].size = flags.size;
	gdt32_table[n].long_mode = flags.long_mode;
	gdt32_table[n].reserved = 0;

	gdt32_table[n].accessed = access.accessed;
	gdt32_table[n].read_write = access.read_write;
	gdt32_table[n].conforming_direction = access.conforming_direction;
	gdt32_table[n].executable = access.executable;
	gdt32_table[n].type = access.type;
	gdt32_table[n].privilege_level = access.privilege_level;
	gdt32_table[n].present = access.present;
}
void manager_t::init_entries()
{
    // null descriptor
	set_desc(0, 0, 0, (gdt32_entry_access){
		.accessed = 0,
		.read_write = 0,
		.conforming_direction = 0,
		.executable = 0,
		.type = 0,
		.privilege_level = 0,
		.present = 0
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_16BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_1B
	});

	// kernel code segment
	set_desc(KERNEL_CODE_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access)
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 1,
		.type = 1,
		.privilege_level = 0,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});
	// kernel data segment
	set_desc(KERNEL_DATA_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access) 
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 0,
		.type = 1,
		.privilege_level = 0,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});

	// module code segment
	set_desc(MODULE_CODE_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access)
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 1,
		.type = 1,
		.privilege_level = 1,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});
	// module data segment
	set_desc(MODULE_DATA_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access)
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 0,
		.type = 1,
		.privilege_level = 1,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});

	// driver code segment
	set_desc(DRIVER_CODE_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access)
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 1,
		.type = 1,
		.privilege_level = 2,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});
	// driver data segment
	set_desc(DRIVER_DATA_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access)
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 0,
		.type = 1,
		.privilege_level = 2,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});

	// kernel code segment
	set_desc(USER_CODE_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access)
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 1,
		.executable = 1,
		.type = 1,
		.privilege_level = 3,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});
	// kernel data segment
	set_desc(USER_DATA_INDEX, 0, 0xFFFFFFFF, (gdt32_entry_access) 
	{
		.accessed = 0,
		.read_write = 1,
		.conforming_direction = 0,
		.executable = 0,
		.type = 1,
		.privilege_level = 3,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_32BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_4K
	});

	// set TSS
	set_desc(TSS_INDEX, (uint32_t)&__tss, sizeof(tss_entry_struct), (gdt32_entry_access)
	{
		.accessed = 1,
		.read_write = 0,
		.conforming_direction = 0,
		.executable = 1,
		.type = 0,
		.privilege_level = 0,
		.present = 1
	}, (gdt32_entry_flags){
		.reserved = 0,
		.long_mode = 0,
		.size = gdt32_size_bit::GDT32_16BIT,
		.granularity = gdt32_granularity_bit::GDT32_ALIGN_1B
	});
	memset(&__tss, 0, sizeof(tss_entry_struct));

	__tss.ss0  = (KERNEL_DATA_INDEX * 8) | 0;
	__tss.ss1  = (MODULE_DATA_INDEX * 8) | 1;
	__tss.ss2  = (DRIVER_DATA_INDEX * 8) | 2;
	int esp;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	__tss.esp0 = (uint32_t) esp;
	__tss.cs = 0x8;
	__tss.ds = 0x10;
	__tss.es = 0x10;
	__tss.fs = 0x10;
	__tss.gs = 0x10;
	__tss.ss = 0x10;
}
void manager_t::load_gdt()
{
    gdt32_table = (gdt32_entry *)malloc(sizeof(gdt32_entry) * GDT_ENTRIES);
	gdt32_register gdtr = {};
    __gdtr = &gdtr;

    init_entries();

    gdtr.base = (uintptr_t) gdt32_table;
    gdtr.limit = sizeof(gdt32_entry) * GDT_ENTRIES;

    __gdt_flush();
    __reload_regs();
}