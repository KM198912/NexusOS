#pragma once

#include <stdint.h>

struct multiboot_module_entry {
    uint32_t start;
    uint32_t end;
    uint32_t string_addr;
    uint32_t reserved;
};
typedef struct multiboot_module_entry multiboot_module_entry_t;

struct multiboot_aout_symbol_table {
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

struct multiboot_elf_section_header_table {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
#define MULTIBOOT_MEMORY_BADRAM 5

struct multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));

typedef struct multiboot_mmap_entry multiboot_memory_map_t;

#define MULTIBOOT_INFO_FRAMEBUFFER_INFO (1 << 12)

struct multiboot_info {
    // Multiboot info version number.
    uint32_t flags;

    // Available memory from BIOS.
    uint32_t mem_lower;
    uint32_t mem_upper;

    // "root" partition.
    uint32_t boot_device;

    // Kernel command line.
    uint32_t cmdline;

    // Boot-Module list.
    uint32_t mods_count;
    uint32_t mods_addr;

    union {
        multiboot_aout_symbol_table_t aout_sym;
        multiboot_elf_section_header_table_t elf_sec;
    } u;

    // Memory Mapping buffer.
    uint32_t mmap_length;
    uint32_t mmap_addr;

    // Drive Info buffer.
    uint32_t drives_length;
    uint32_t drives_addr;

    // ROM configuration table.
    uint32_t config_table;

    // Boot Loader Name.
    uint32_t boot_loader_name;

    // APM table.
    uint32_t apm_table;

    // Video.
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
    uint8_t framebuffer_type;
    union {
        struct
        {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        };
        struct
        {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
};
typedef struct multiboot_info multiboot_info_t;
