#pragma once

#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>

#define KERNEL_CODE_INDEX 1
#define KERNEL_DATA_INDEX 2

#define USER_CODE_INDEX   3
#define USER_DATA_INDEX   4

#define TSS_INDEX 5

#define MODULE_CODE_INDEX 6
#define MODULE_DATA_INDEX 7

#define DRIVER_CODE_INDEX 8
#define DRIVER_DATA_INDEX 9

#define GDT_ENTRIES 10

namespace system::hal::interrupts::gdt
{
    enum class gdt32_direction_bit
    {
        GDT32_DIR_UPWARDS = 0,
        GDT32_DIR_DOWNWARDS = 1
    };
    enum class gdt32_conforming_bit
    {
        GDT32_CONF 	  = 0,
        GDT32_NONCONF = 1
    };
    enum class gdt32_granularity_bit
    {
        GDT32_ALIGN_1B = 0,
        GDT32_ALIGN_4K = 1
    };
    enum class gdt32_size_bit
    {
        GDT32_16BIT = 0,
        GDT32_32BIT = 1
    };


    struct gdt32_entry_access
    {
        uint8_t accessed : 1;
        uint8_t read_write : 1;
        uint8_t conforming_direction : 1;
        uint8_t executable : 1;
        uint8_t type : 1;
        uint8_t privilege_level : 2;
        uint8_t present : 1;
    } packed__;
    struct gdt32_entry_flags
    {
        uint8_t reserved : 1;
        bool long_mode : 1;
        gdt32_size_bit size : 1;
        gdt32_granularity_bit granularity : 1;		
    } packed__;

    struct gdt32_entry
    {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_middle;
        
        uint8_t accessed : 1;
        uint8_t read_write : 1;
        uint8_t conforming_direction : 1;
        uint8_t executable : 1;
        uint8_t type : 1;
        uint8_t privilege_level : 2;
        uint8_t present : 1;
        
        uint8_t limit_high : 4; // aka limit_high/flags

        uint8_t reserved : 1;
        bool long_mode : 1;
        gdt32_size_bit size : 1;
        gdt32_granularity_bit granularity : 1;		

        uint8_t base_high;
    } packed__;
    struct gdt32_register
    {
        uint16_t limit;
        uint32_t base;
    } packed__;

    struct tss_entry_struct
    {
        uint32_t prev_tss;
        uint32_t esp0;
        uint32_t ss0;
        uint32_t esp1;
        uint32_t ss1;
        uint32_t esp2;
        uint32_t ss2;
        uint32_t cr3;
        uint32_t eip;
        uint32_t eflags;
        uint32_t eax;
        uint32_t ecx;
        uint32_t edx;
        uint32_t ebx;
        uint32_t esp;
        uint32_t ebp;
        uint32_t esi;
        uint32_t edi;
        uint32_t es;
        uint32_t cs;
        uint32_t ss;
        uint32_t ds;
        uint32_t fs;
        uint32_t gs;
        uint32_t ldt;
        uint16_t trap;
        uint16_t iomap_base;
    } packed__;

    __cdecl gdt32_register *__gdtr;
    __cdecl gdt32_entry *gdt32_table;
    __cdecl tss_entry_struct *tss_sect;
    __cdecl tss_entry_struct __tss;

    __cdecl void __reload_regs();
    __cdecl void __gdt_flush();

    class manager_t final
    {
    private:
        static void set_desc(int n, uint32_t base, uint32_t limit, gdt32_entry_access access, gdt32_entry_flags flags);
        static void init_entries();
    
    public:
        static void load_gdt();

    public:
        static inline uint16_t get_kernel_code_seg()
        {
            return (KERNEL_CODE_INDEX * 8) | 0;
        }
        static inline uint16_t get_kernel_data_seg()
        {
            return (KERNEL_DATA_INDEX * 8) | 0;
        }
    public:
        static inline uint16_t get_module_code_seg()
        {
            return (MODULE_CODE_INDEX * 8) | 1;
        }
        static inline uint16_t get_module_data_seg()
        {
            return (MODULE_DATA_INDEX * 8) | 1;
        }
    public:
        static inline uint16_t get_driver_code_seg()
        {
            return (DRIVER_CODE_INDEX * 8) | 2;
        }
        static inline uint16_t get_driver_data_seg()
        {
            return (DRIVER_DATA_INDEX * 8) | 2;
        }
    public:
        static inline uint16_t get_user_code_seg()
        {
            return (USER_CODE_INDEX * 8) | 3;
        }
        static inline uint16_t get_user_data_seg()
        {
            return (USER_DATA_INDEX * 8) | 3;
        }
    public:
        static inline uint16_t get_tss_seg()
        {
            return (TSS_INDEX * 8) | 0;
        }
    };
}