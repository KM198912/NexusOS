#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
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
            private:
                int IsTransMitEmpty();
                void WriteChar(char c);
        };
    }
}