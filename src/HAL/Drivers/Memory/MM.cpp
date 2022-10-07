#include <HAL/Drivers/Memory/MM.hpp>
#include <core/main.hpp>
#include <lib/string.hpp>
namespace System
{
    namespace Memory
    {
        void MM::Initialize(void* end)
        {
            start = end;
            current = end;
            memset(start, 0, 0x100000);

        }
        void* MM::Allocate(size_t size)
        {
            MemoryBlock* block = (MemoryBlock*)current;
            block->ptr = current + sizeof(MemoryBlock);
            block->size = size;
            block->next = NULL;
            current = current + sizeof(MemoryBlock) + size;
            return block->ptr;
        }
        void* MM::Resize(void* ptr, size_t size)
        {
            MemoryBlock* block = (MemoryBlock*)ptr;
            block->size = size;
            block->next = (MemoryBlock*)((uint32_t)ptr + size);
            current = block->next;
            return block->ptr;
        }
        void MM::Free(void* ptr)
        {
            MemoryBlock* block = (MemoryBlock*)ptr;
            block->ptr = NULL;
            block->size = 0;
            block->next = NULL;
        }
    }
}