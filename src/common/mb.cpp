#include <common/mb.h>

namespace System
{
    namespace Multiboot
    {
        void Manager::Copy(multiboot_info_t* mbb)
        {
            store = mbb;
        }
        multiboot_info_t* Manager::Parse()
        {
            return store;
        }
    }
}