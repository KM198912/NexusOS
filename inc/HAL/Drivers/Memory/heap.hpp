#pragma once
#include <stdint.h>
#include <HAL/Drivers/Memory/memmgr.hpp>
#include <std/decl.h>

namespace System
{
    namespace Memory {
    /// @brief Structure for managing heap allocations
    typedef struct
    {
        /// @brief Physical address of allocation in memory
        uint32_t  address;
        /// @brief Size of allocation in bytes
        size_t size;
        /// @brief Type of allocation
        ALLOCTYPE type;
    } packed__ heapblock_t;

    class memory_heap
    {   
        /// @internal Allow access to private members
        friend class garbage_collector;

        private:
            /// @brief List of heap allocations
            heapblock_t* _blocks;
            /// @brief Amount of exising entries - All types except 'invalid' are included in summary
            size_t _count;
            /// @brief Maximum amount of heap allocations
            size_t _count_max;
            /// @brief Size of total heap data in bytes
            size_t _size;
            /// @brief Allocation alignment
            uint32_t _alignment;
            /// @brief Starting address of allocation data
            uint32_t _data_start;
            /// @brief Debugging messages
            bool _messages;

        public:
            /// @brief Initialize kernel heaps @param messages Toggle debugging messagse
            static void init(bool messages);

            /// @brief Initialize heap instance @param size Size of heap data in bytes @param count Maximum amount of allocations @param align Allocation alignment @param msgs Debugging messages
            void init(size_t size, size_t count, uint32_t align, bool msgs = true);

        public:
            /// @brief Request to allocate block of memory @param size Size in bytes @param type Allocation type @param clear Zero memory block after allocating @return Pointer to allocation
            void* allocate(size_t size, ALLOCTYPE type, bool clear = false);
            /// @brief Free existing allocation @param ptr Pointer to allocation @return Allocation freed successfully
            bool free(void* ptr);
            /// @brief Merge free heap entries @return Amount of entries merged
            uint32_t merge();
            /// @brief Print list heap entries
            void print();
        
        public:
            /// @brief Create a new heap entry @param addr Address value @param thread Pointer to thread @param size Size in bytes @param type Allocation type @return Pointer to heap entry
            heapblock_t* create(uint32_t addr, size_t size, ALLOCTYPE type);
            /// @brief Remove exisitng heap entry @param blk Pointer to heap entry @return Entry removed successfully
            bool remove(heapblock_t* blk);

        public:
            /// @brief Attempt to locate free heap entry with specified size @param size Size in bytes @return Pointer to heap entry
            heapblock_t* next(size_t size);
            /// @brief Get next unused entry in list @return Pointer to heap entry
            heapblock_t* next_empty();
            /// @brief Get heap heap entry in memory @param blk Pointer to heap entry @return Pointer to nearest entry
            heapblock_t* nearest(heapblock_t* blk);
            /// @brief Get heap entry from address @param ptr Pointer to allocation @return Pointer to heap entry
            heapblock_t* fetch(void* ptr);
            /// @brief Validate address @param addr Address value @return Address is within heap bounds
            bool validate(uint32_t addr);
            /// @brief Validate address @param addr Address pointer @return Pointer is within heap bounds
            bool validate(void* ptr);

        public:
            /// @brief Toggle debugging messages @param state Debugging enabled
            void toggle_msgs(bool state);
            /// @brief Calculate amount of used memory in bytes @return Size in bytes
            size_t calc_used();
            /// @brief Calculate amount of used memory in bytes for specified thread @param thread Pointer to thread @return Size in bytes
         //   size_t calc_used(threading::thread_t* thread);
            /// @brief Get name from allocation type @param type Allocation type @return Pointer to type string
            static const char* typestr(ALLOCTYPE type);

        public:
            /// @brief Get total size of heap including entry table
            uint32_t total_size();
            /// @brief Get size of heap in bytes
            uint32_t data_size();
            /// @brief Get size of table in bytes
            uint32_t table_size();
            /// @brief Get allocation alignment
            uint32_t alignment();
            /// @brief Get amount of entries
            uint32_t count();
            /// @brief Get maximum amount of entries
            uint32_t count_max();
            /// @brief Get list of heap entries
            heapblock_t* blocks();
    };
}
}