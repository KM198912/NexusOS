#include <core/main.hpp>
#include <common/mb.h>
#include <HAL/Drivers/Video/FB.hpp>
#include <HAL/Drivers/Memory/MM.hpp>
#include <lib/colors.hpp>
#include <lib/string.hpp>
#include <Debug/Serial.hpp>
extern "C" {
    extern uint32_t kernel_end;
}
namespace System
 {

    namespace Kernel
    {
        System::HAL::Video::VESA VESA;
        System::Memory::MM MM;
        System::Multiboot::Manager manager;
        System::Debug::Serial serial;
        void Boot(multiboot_info_t* mb)
        {
            serial.Initialize();
            serial.Printf("Bootloader: %s\n", mb->boot_loader_name);
            manager.Copy(mb);
            MM.Initialize(&kernel_end);
            VESA.Initialize();
            char* test = (char*)MM.Allocate(10);
            strcpy(test, "Hello");
            serial.Printf("Test: %s\n", test);
            MM.Free(test);
            serial.Printf("Test: %s\n", test);
        }
        void Run()
        {
            VESA.Clear((uint32_t)Color::DarkCyan);
            VESA.Update();
        }
    }
 }