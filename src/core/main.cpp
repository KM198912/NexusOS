#include <core/main.hpp>
#include <common/mb.h>
#include <HAL/Drivers/Video/FB.hpp>
#include <HAL/Drivers/Memory/MM.hpp>
#include <lib/colors.hpp>
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
        void Boot(multiboot_info_t* mb)
        {
            manager.Copy(mb);
            MM.Initialize(&kernel_end);
            VESA.Initialize();
        }
        void Run()
        {
            VESA.Clear((uint32_t)Color::DarkCyan);
        }
    }
 }