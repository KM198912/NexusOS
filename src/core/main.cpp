#include <core/main.hpp>
#include <common/mb.h>
#include <HAL/Drivers/Video/FB.hpp>
#include <HAL/Drivers/Memory/heap.hpp>
#include <HAL/Drivers/Memory/memmgr.hpp>
#include <lib/colors.hpp>
#include <lib/string.hpp>
#include <Debug/Serial.hpp>
#include <ints/gdt.hpp>
#include <ints/idt.hpp>
extern "C" {
    extern uint32_t kernel_end;
    extern uint32_t kernel_start;
    extern uint32_t _bss_start;
    extern uint32_t _bss_end;
    extern uint32_t _stack_bottom;
    extern uint32_t _stack_top;
}
namespace System
 {
    namespace Kernel
    {
        System::HAL::Video::VESA VESA;
        System::Multiboots::Manager manager;
        System::Debug::Serial serial;
        System::Terminal term;
        Memory::memory_heap           heap_small;
        Memory::memory_heap           heap_large;
        Memory::Paging paging;
        using namespace system::hal::interrupts;
        void _Prepare(multiboot_info_t* mb)
        {
            asm volatile("cli");
            serial.Initialize();
            manager.Copy(mb);
            io_wait();
            Memory::memory_manager::init();
            bool mmdbg = true;
            Memory::memory_heap::init(mmdbg);
            term.Initialize();
            term.Clear();
            gdt::manager_t::load_gdt();
            idt::manager_t::init();
            idt::manager_t::enable_interrupts();
            
        }
        void Ready()
        {
            term.Clear();
            term.Printf("Welcome to the kernel!\n");
            int i = 0;
            int a = 1;
            int panic = a/i;
            term.Printf("Panic: %d",panic);
            
        }
        void Run()
        {
            term.Render();
        }
        uint32_t StartAddress() { return (uint32_t)&kernel_start; }
        uint32_t EndAddress() { return (uint32_t)&kernel_end; }
        uint32_t Size() { return (uint32_t)&kernel_end - (uint32_t)&kernel_start; }
    }
 }