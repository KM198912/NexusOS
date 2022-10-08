#pragma once
#include <common/mb.h>
#include <stdint.h>
#include <lib/colors.hpp>
#include <lib/font.hpp>
#define DEBUG_INFO   "[  \x1b[36m>>\x1b[0m  ]"
#define DEBUG_OK     "[  \x1b[32mOK\x1b[0m  ]"
#define DEBUG_WARN   "[  \x1b[33m??\x1b[0m  ]"
#define DEBUG_ERROR  "[  \x1b[31m!!\x1b[0m  ]"
#define DEBUG_MMAP   "[ \x1b[36mMMAP\x1b[0m ]"
#define DEBUG_PMAP   "[ \x1b[35mPMAP\x1b[0m ]"
#define DEBUG_PUNMAP "[\x1b[33mPUNMAP\x1b[0m]"
#define DEBUG_VMAP   "[ \x1b[35mVMAP\x1b[0m ]"
#define DEBUG_VUNMAP "[\x1b[33mVUNMAP\x1b[0m]"
#define DEBUG_MALLOC "[\x1b[35mMALLOC\x1b[0m]"
#define DEBUG_FREE   "[ \x1b[33mFREE\x1b[0m ]"
#define DEBUG_KALLOC "[\x1b[35mKALLOC\x1b[0m]"
#define DEBUG_KDEL   "[ \x1b[33mKDEL\x1b[0m ]"
#define DEBUG_THREAD "[\x1b[35mTHREAD\x1b[0m]"
#define DEBUG_PROC   "[ \x1b[35mPROC\x1b[0m ]"
#define DEBUG_KILLED "[\x1b[33mKILLED\x1b[0m]"

#define DEBUG_INFO_VBE   "[  >>  ]"
#define DEBUG_OK_VBE     "[  OK  ]"
#define DEBUG_WARN_VBE   "[  ??  ]"
#define DEBUG_ERROR_VBE  "[  !!  ]"
#define DEBUG_MMAP_VBE   "[ MMAP ]"
#define DEBUG_PMAP_VBE   "[ PMAP ]"
#define DEBUG_PUNMAP_VBE "[PUNMAP]"
#define DEBUG_VMAP_VBE   "[ VMAP ]"
#define DEBUG_VUNMAP_VBE "[VUNMAP]"
#define DEBUG_MALLOC_VBE "[MALLOC]"
#define DEBUG_FREE_VBE   "[ FREE ]"
#define DEBUG_KALLOC_VBE "[KALLOC]"
#define DEBUG_KDEL_VBE   "[ KDEL ]"
#define DEBUG_THREAD_VBE "[THREAD]"
#define DEBUG_PROC_VBE   "[ PROC ]"
#define DEBUG_KILLED_VBE "[KILLED]"

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
                    void Clear(Color color);
                    void DrawFilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
                    void DrawOutlineRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
                    uint32_t GetWidth();
                    uint32_t GetHeight();
                    void DrawChar(uint16_t x, uint16_t y, char c, Color fg, Color bg, Font font);
                    void DrawString(uint16_t x, uint16_t y, const char* str, Color fg, Color bg, Font font);
                    void Update();
                    void Map();
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