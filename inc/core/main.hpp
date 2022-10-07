#pragma once
#include <stdint.h>
#include <stddef.h>
#include <common/multiboot.h>
#include <common/mb.h>
namespace System
{
    namespace Kernel
    {
        extern System::Multiboot::Manager manager;
        void Boot(multiboot_info_t* mb);
        void Run();
    }
}