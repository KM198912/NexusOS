#include <Debug/Serial.hpp>
#include <lib/ports.hpp>
#include <lib/string.hpp>
#include <core/main.hpp>
namespace System
{
    namespace Debug
    {
        void Serial::Initialize()
        {
            outb(SERIAL_PORT + 1, 0x00);    // Disable all interrupts
            outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
            outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
            outb(SERIAL_PORT + 1, 0x00);    //                  (hi byte)
            outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
            outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
            outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
        }
        void Serial::Write(const char* str)
        {
            for (size_t i = 0; str[i] != '\0'; i++)
            {
                WriteChar(str[i]);
            }
        }
        void Serial::Printf(const char* str, ...)
        {
            va_list args;
            va_start(args, str);
            char* buffer = (char*)System::Kernel::MM.Allocate(1024);
            vsprintf(buffer, str, args);
            Write(buffer);
            va_end(args);
        }
        int Serial::IsTransMitEmpty()
        {
            return inb(SERIAL_PORT + 5) & 0x20;
        }
        void Serial::WriteChar(char c)
        {
            while (IsTransMitEmpty() == 0);
            outb(SERIAL_PORT, c);
        }
        

    }
}