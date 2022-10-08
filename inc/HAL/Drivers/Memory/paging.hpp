#pragma once
#include <stdint.h>
namespace System
{
    namespace Memory
    {
        class Paging
        {
            public:
                void Initialize();
                void Map(uint32_t virt, uint32_t phys);
            private:
                void Enable();

        };
    }
}