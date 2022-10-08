#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <lib/colors.hpp>
namespace System
{
    class Terminal
    {
        public:
            void Initialize();
            void WriteChar(char c);
            void WriteChar(Color FG,Color BG,char c);
            void Write(const char* str);
            void Write(Color fg,const char* str);
            void Write(Color fg,Color bg,const char* str);
            void Clear();
            void Printf(const char* str, ...);
            void Printf(Color FG,Color BG,const char* str, ...);
            Color GetFG();
            Color GetBG();
            void SetFG(Color col);
            void SetBG(Color col);
            void SetColors(Color fg,Color bg);
            void Render();
            bool IsInitialized();
        private:
            uint32_t cursorX;
            uint32_t cursorY;
            Color CurCol;
            Color CurBG;
            bool Initialized;

    };
}