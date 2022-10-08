#include <common/mb.h>

namespace System
{
    namespace Multiboots
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