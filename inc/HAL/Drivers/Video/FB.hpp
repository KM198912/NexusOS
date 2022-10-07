#pragma once
#include <common/mb.h>
#include <stdint.h>
namespace System
{
    namespace HAL
    {
        namespace Video
        {
            class VESA
            {
                public:
                    void Initialize();
                    void SetPixel(uint32_t x, uint32_t y, uint32_t color);
                    void Clear();
                    void Clear(uint32_t color);
                    void Update();
                private:
                    uint32_t* buffer;
                    uint32_t  width;
                    uint32_t  height;
                    uint32_t  pitch;
                    uint32_t  size;
            };
        }
    }
}