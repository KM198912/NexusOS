#pragma once
#include <stdint.h>
#include <stddef.h>
#include <common/multiboot.h>
#include <common/mb.h>
#include <HAL/Drivers/Video/FB.hpp>
#include <HAL/Drivers/Memory/heap.hpp>
#include <HAL/Drivers/Memory/paging.hpp>
#include <Debug/Serial.hpp>
#include <lib/colors.hpp>
#include <core/term.hpp>
#define KB (1024)
#define MB (KB * 1024)
#define GB (MB * 1024)
namespace System
{
        
    namespace Kernel
    {
        extern Memory::memory_heap           heap_small;
        extern Memory::memory_heap           heap_large;
        extern Memory::memblk_t              heap_small_blocks[1024];
        extern System::HAL::Video::VESA VESA;
        extern System::Multiboots::Manager manager;
        extern System::Debug::Serial serial;
        extern System::Memory::Paging paging;
        extern System::Terminal term;
        void _Prepare(multiboot_info_t* mb);
        void Ready();
        void Run();
        uint32_t StartAddress();
        uint32_t EndAddress();
        uint32_t Size();
    }
}