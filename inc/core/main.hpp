#pragma once
#include <stdint.h>
#include <stddef.h>
#include <common/multiboot.h>
#include <common/mb.h>
#include <HAL/Drivers/Video/FB.hpp>
#include <HAL/Drivers/Memory/MM.hpp>
#include <Debug/Serial.hpp>
#include <lib/colors.hpp>
namespace System
{
    namespace Kernel
    {
        extern System::HAL::Video::VESA VESA;
        extern System::Memory::MM MM;
        extern System::Multiboot::Manager manager;
        extern System::Debug::Serial serial;
        void Boot(multiboot_info_t* mb);
        void Run();
    }
}