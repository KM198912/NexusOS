#include <HAL/Drivers/Memory/MM.hpp>
namespace System
{
    namespace Memory
    {
        void MM::Initialize(void* end)
        {
            start = (void*)end;
            current = start;
            end = (void*)0xFFFFFFFF;
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