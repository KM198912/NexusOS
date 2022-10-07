#include <core/main.hpp>
#include <common/mb.h>
#include <HAL/Drivers/Video/FB.hpp>
namespace System
 {

    namespace Kernel
    {
        System::HAL::Video::VESA VESA;
        System::Multiboot::Manager manager;
        void Boot(multiboot_info_t* mb)
        {
            manager.Copy(mb);
            VESA.Initialize();
        }
        void Run()
        {
            VESA.Clear(0xFFFFFFFF);
        }
    }
 }