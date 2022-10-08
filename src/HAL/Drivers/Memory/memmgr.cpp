#include <HAL/Drivers/Memory/memmgr.hpp>
#include <core/main.hpp>
#include <lib/string.hpp>
#define MEMBLK_COUNT 1024

namespace System
{
    namespace Memory {
    bool memory_manager::messages;
    
    memblk_t  memory_manager::_memblks[MEMBLK_COUNT];

    memblk_t* memory_manager::_freeblk;

    memblk_t* memory_manager::_after_freeblk;

    size_t memory_manager::_usable;

    void memory_manager::init()
    {
        memset(&_memblks, 0, sizeof(memblk_t) * MEMBLK_COUNT);
        _freeblk       = NULL;
        _after_freeblk = NULL;
        _usable        = 0;
        messages       = false;
        
        probe_mmap();
        probe_mods();

        printf("%s Zeroing free memory from 0x%x to 0x%x\n", DEBUG_INFO, _freeblk->address, _freeblk->address + _freeblk->size);
        memset((void*)_freeblk->address, 0, _freeblk->size);
        
        _usable = _freeblk->size;

        messages = true;
        print();
        printf("%s Initialized memory manager\n", DEBUG_OK);
    }

    void memory_manager::probe_mmap()
    {
            multiboot_memory_map_t* mmap  = (multiboot_memory_map_t*)Kernel::manager.Parse()->mmap_addr;
            size_t             count = (size_t)System::Kernel::manager.Parse()->mmap_length / sizeof(multiboot_memory_map_t);

            for (size_t i = 0; i < count; i++)
            {
                if (mmap[i].type == 0) { continue; }
                if (mmap[i].addr < Kernel::StartAddress() && mmap[i].type == (uint32_t)memblk_type::available) { continue; }
                memblk_t* blk = blk = map(mmap[i].addr, mmap[i].len,(memblk_type)mmap[i].type);
                if (mmap[i].addr > Kernel::StartAddress() && mmap[i].type == (uint32_t)memblk_type::reserved && _after_freeblk == NULL) { _after_freeblk = blk;
                }
                if (blk->address == Kernel::StartAddress()) { _freeblk = blk; 
                }
            }

            if (_freeblk == NULL) { perror("Failed to parse memory map\n"); }
            _freeblk->address += (Kernel::EndAddress() - Kernel::StartAddress());
            _freeblk->size    -= (Kernel::EndAddress() - Kernel::StartAddress());
            map(Kernel::StartAddress(), Kernel::Size(), memblk_type::kernel);
            printf("%s Finished fetching memory map entries\n", DEBUG_INFO);
    }

    void memory_manager::probe_mods()
    {
        //System::MultibootModule* mods = Multiboot.ModulesAddress;
        multiboot_module_entry_t* mods = (multiboot_module_entry_t*)System::Kernel::manager.Parse()->mods_addr;
        if (mods == NULL) { return; }

        uint32_t mods_end  = 0;
        uint32_t mods_size = 0;
        for (size_t i = 0; i < System::Kernel::manager.Parse()->mods_count; i++)
        {
            if (mods[i].start == 0) { continue; }
            uint32_t end = memalign(mods[i].end, 0x1000);
            map(mods[i].start, end - mods[i].end, memblk_type::module);
            mods_end = end;
            mods_size += (end - mods[i].start);
        }

        _freeblk->address  = mods_end;
        _freeblk->size     = (_after_freeblk->address - _freeblk->address); // + (kernel::end_addr() - kernel::start_addr());
    }

    memblk_t* memory_manager::map(uint32_t addr, size_t size, memblk_type type)
    {
        if (size == 0 || type == memblk_type::invalid) { perror("Tried to map invalid memory block - ADDR:0x%x TYPE:0x%2x SIZE:%d bytes", addr, type, size); return NULL; }

        memblk_t* blk = next();
        if (blk == NULL) { perror("Maximum amount of memory blocks reached"); return NULL; }
        *blk = (memblk_t){ .address = addr, .size = size, .type = type };
        if (messages) { printf("%s TYPE:%s ADDR:0x%x SIZE:%d bytes\n", DEBUG_PMAP, typestr(type), addr, size); }
        return blk;
    }

    memblk_t* memory_manager::request(size_t size, memblk_type type)
    {
        if (size == 0) { perror("Attempt to request 0 bytes of physical memory"); return NULL; }
        if (type == memblk_type::invalid || type == memblk_type::bad_ram) { perror("Attempt to request physical memory of invalid type"); return NULL; }

        uint32_t avail = 0;
        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].type == memblk_type::available)
            {   
                avail = _memblks[i].size;
                if (_memblks[i].size >= size)
                {
                    memblk_t* blk = map(_memblks[i].address, size, type);
                    _memblks[i].address += size;
                    _memblks[i].size    -= size;
                    return blk;
                }
            }
        }

        perror("Failed to request %d bytes of physical memory - available: %d bytes", size, avail);
        return NULL;
    }

    memblk_t* memory_manager::first_bytype(memblk_type type)
    {
        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].type == type) { return &_memblks[i]; }
        }
        return NULL;
    }

    memblk_t* memory_manager::next()
    {
        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].type == memblk_type::invalid) { return &_memblks[i]; }
        }
        return NULL;
    }

    size_t memory_manager::usable() { return _usable; }
    
    const char* memory_manager::typestr(memblk_type type)
    {
        switch (type)
        {
            default:                            { return "invalid     "; }
            case memblk_type::available:        { return "available   "; }
            case memblk_type::reserved:         { return "reserved    "; }
            case memblk_type::acpi_reclaimable: { return "acpi reclaim"; }
            case memblk_type::nvs:              { return "nvs         "; }
            case memblk_type::bad_ram:          { return "bad ram     "; }
            case memblk_type::kernel:           { return "kernel      "; }
            case memblk_type::heap:             { return "heap        "; }
            case memblk_type::module:           { return "module      "; }
        }
    }

    void memory_manager::print()
    {
        printf("%s Showing list of memory blocks\n", DEBUG_INFO);
        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].type == memblk_type::invalid) { continue; }
            printf("%s ADDR:0x%x TYPE:%s SIZE:%d bytes\n", DEBUG_MMAP, _memblks[i].address, typestr(_memblks[i].type), _memblks[i].size);
        }
    }
}
}
void *operator new(size_t size) { return tmalloc(size, ALLOCTYPE_OBJ); }
void *operator new[](size_t size) { return tmalloc(size, ALLOCTYPE_ARRAY); }
void operator delete(void *p) { free(p); }
void operator delete(void *p, size_t size) { free(p); UNUSED(size); }
void operator delete[](void *p) { free(p); }
void operator delete[](void *p, size_t size) { free(p); UNUSED(size); }
