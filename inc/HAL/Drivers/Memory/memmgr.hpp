#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <std/decl.h>
namespace System
{
    namespace Memory {
    /// @brief Memory block types
    enum class memblk_type : uint8_t
    {
        /// @brief Invalid type
        invalid,
        /// @brief Available for use
        available,
        /// @brief Reserved for system
        reserved,
        /// @brief ACPI reclaimable
        acpi_reclaimable,
        /// @brief ACPI NVS
        nvs,
        /// @brief Bad or damaged memory
        bad_ram,
        /// @brief Kernel
        kernel,
        /// @brief Kernel module
        module,
        /// @brief Heap
        heap,
    };

    /// @brief Structure for managing physical memory blocks
    typedef struct
    {
        /// @brief Physical address of block
        uint32_t    address;
        /// @brief Size in bytes
        size_t      size;
        /// @brief Type of memory
        memblk_type type;
    }packed__ memblk_t;

    /// @brief Physical memory management service
    class memory_manager
    {
        public:
            /// @brief Toggle debugging messages
            static bool messages;

        private:
            /// @brief List of memory blocks
            static memblk_t  _memblks[];
            /// @brief Available memory block pointer
            static memblk_t* _freeblk;
            /// @brief Memory block pointer for block after available
            static memblk_t* _after_freeblk;
            /// @brief Amount of usable memory in bytes - only valid immediately after initialization
            static size_t _usable;

        public:
            /// @brief Initialize physical memory manager
            static void init();
            /// @brief Print physical memory blocks
            static void print();
            /// @brief Parse grub memory map and transfer entries to list
            static void probe_mmap();
            /// @brief Parse grub modules and transfer entires to list
            static void probe_mods();

        public:
            /// @brief Map physical block of memory @param addr Address value @param size Size in bytes @param type Memory block type
            static memblk_t* map(uint32_t addr, size_t size, memblk_type type);
            /// @brief Request memory blok of specified size @param size Size in bytes @param type Memory block type to return @return Pointer to memory block
            static memblk_t* request(size_t size, memblk_type type = memblk_type::reserved);
            /// @brief Get first memory block of specified type @param type Memory block type @return Pointer to memory block
            static memblk_t* first_bytype(memblk_type type);

        public:
            /// @brief Get amount of usable memory in bytes @return Size in bytes
            static size_t usable();
            /// @brief Get name from memory block type @param type Memory block type @return Pointer to type string
            static const char* typestr(memblk_type type);
            
        private:
            /// @brief Get next unused memory block in list @return Pointer to memory block
            static memblk_t* next();
            
    };
}
}
        // allocate overloads
        extern void *operator new(size_t size);
        extern void *operator new[](size_t size);

        // delete overloads
        extern void operator delete(void *p);
        extern void operator delete(void *p, size_t size);
        extern void operator delete[](void *p);
        extern void operator delete[](void *p, size_t size);