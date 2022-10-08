#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <lib/colors.hpp>
#define SERIAL_PORT 0x3F8
namespace System
{
    namespace Debug
    {
        class Serial
        {
            public:
                void Initialize();
                void Write(const char* str);
                void Printf(const char* str, ...);
                void WriteChar(char c);
            private:
                int IsTransMitEmpty();
                
        };
    }
}
extern "C" {
    void perror(const char* str,...);
    void printf(const char* str,...);
}