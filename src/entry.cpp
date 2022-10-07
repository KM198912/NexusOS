#include <core/main.hpp>
extern "C" {
void kmain(uintptr_t stack_top,uintptr_t stack_bottom,multiboot_info_t* mb, uint32_t magic)
{
    System::Kernel::Boot(mb);
    
for(;;)
{
    System::Kernel::Run();
}
}
}
