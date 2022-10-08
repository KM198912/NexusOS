/*
Loader by: Nicola Leone Ciardi and Kevin Meerts
(C) Copyright 2022, All rights reserved.
*/
#pragma once
#include <std/decl.h>
#include <stddef.h>
#include <stdint.h>
#include <boot/options.h>

#define LOADER_MAGIC 0x1BADB002

struct multiboot_header_loader
{
    uint32_t magic = LOADER_MAGIC;
    struct
    {
        bool align: 1;
        bool mmap: 1;
        bool video: 1;
        uint32_t : 29;
    } flags;
    uint32_t checksum;
    uint8_t  reserved[20] = {0};
    struct
    {
        uint32_t video_mode;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    } vbe_info;
};
struct memory_regions
{
    uint8_t *kernel_stack;
    uint8_t *kernel_heap ;
    uint8_t *common_heap ;
    uint8_t *lf_heap     ;
    uintptr_t end;
};

extern memory_regions regions;

#ifndef __cplusplus
typedef struct multiboot_header_loader multiboot_header_t;
typedef struct memory_regions memory_regions_t;
#endif