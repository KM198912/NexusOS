#include <std/decl.h>
#include <core/main.hpp>
__cdecl {
    void kernel_main(multiboot_info_t* mb)
    {
        System::Kernel::_Prepare(mb);
        System::Kernel::Ready();

        while(1) {
            System::Kernel::Run();
        }
    }
}