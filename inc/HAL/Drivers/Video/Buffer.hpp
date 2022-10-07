#pragma once
#include <lib/string.hpp>
namespace System
{
    namespace Video
    {
        class Buffer
        {

            public:
                typedef struct buf
                {
                    uint32_t* buffer;
                    uint32_t  width;
                    uint32_t  height;
                    uint32_t  pitch;
                    uint32_t  size;
                } buf;
            buf Initialize(uint32_t* buffer, uint32_t width, uint32_t height, uint32_t pitch, uint32_t size);
        };
    }
}