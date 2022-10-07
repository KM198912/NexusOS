#pragma once
#include <stdint.h> 
#include <stddef.h>
namespace System
{
    namespace Memory
    {
        class MM
        {
            public:
                void Initialize(void* end);
                void* Allocate(size_t size);
                void* Resize(void* ptr, size_t size);
                void Free(void* ptr);
            private:
                void* start;
                void* end;
                void* current;
            typedef struct MemoryBlock
            {
                void* ptr;
                size_t size;
                MemoryBlock* next;
            } MemoryBlock;
        };
    }
}