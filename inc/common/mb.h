#pragma once
#include <common/multiboot.h>
namespace System
{
    namespace Multiboots
    {
        class Manager
        {
            public:
                void Copy(multiboot_info_t* mbb);
                multiboot_info_t* Parse();
            private:
                multiboot_info_t* store;
        };
    }
}