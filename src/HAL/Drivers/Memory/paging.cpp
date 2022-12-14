#include <HAL/Drivers/Memory/paging.hpp>
#include <core/main.hpp>
namespace System
{
    namespace Memory
    {
        static uint32_t* page_directory = 0;
        static uint32_t page_dir_loc = 0;
        static uint32_t* last_page = 0;

        void Paging::Map(uint32_t virt, uint32_t phys)
        {
            uint16_t id = virt >> 22;
            for(int i = 0; i < 1024; i++)
            {
                last_page[i] = phys | 3;
                phys += 4096;
            }
            page_directory[id] = ((uint32_t)last_page) | 3;
            last_page = (uint32_t *)(((uint32_t)last_page) + 4096);
            printf("Mapping 0x%x (%d) to 0x%x\n", virt, id, phys);
        }

        void Paging::Enable()
        {
            asm volatile("mov %%eax, %%cr3": :"a"(page_dir_loc));	
            asm volatile("mov %cr0, %eax");
            asm volatile("orl $0x80000000, %eax");
            asm volatile("mov %eax, %cr0");
        }

        void Paging::Initialize()
        {
            printf("Setting up paging\n");
            page_directory = (uint32_t*)0x400000;
            page_dir_loc = (uint32_t)page_directory;
            last_page = (uint32_t *)0x404000;
            for(int i = 0; i < 1024; i++)
            {
                page_directory[i] = 0 | 2;
            }
            Map(0, 0);
            Map(0x400000, 0x400000);
            Enable();
            printf("Paging was successfully enabled!\n");
        }
    }
}