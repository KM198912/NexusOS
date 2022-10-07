#include <HAL/Drivers/Memory/MM.hpp>
namespace System
{
    namespace Memory
    {
        void MM::Initialize()
        {
            start = (void*)0x100000;
            end = (void*)0x1000000;
            current = start;
        }
        void* MM::Allocate(size_t size)
        {
            void* ptr = current;
            current = (void*)((uint32_t)current + size);
            return ptr;
        }
        void MM::Free(void* ptr)
        {
            current = ptr;
        }
    }
}