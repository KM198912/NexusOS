#include <core/term.hpp>
#include <core/main.hpp>
#include <lib/string.hpp>

namespace System
{
    void Terminal::Initialize()
    {
        System::Kernel::VESA.Initialize();
        cursorX = 0;
        cursorY = 0;
        CurBG = Color::Black;
        CurCol = Color::White;
        Initialized = true;
    }
    bool Terminal::IsInitialized()
    {
        return Initialized;
    }
    Color Terminal::GetFG()
    {
        return CurCol;
    }
    Color Terminal::GetBG()
    {
        return CurBG;
    }
    void Terminal::SetFG(Color col)
    {
        CurCol = col;
    }
    void Terminal::SetBG(Color col)
    {
        CurBG = col;
    }
    void Terminal::SetColors(Color fg,Color bg)
    {
        System::Kernel::VESA.Clear(bg);
        CurCol = fg;
        CurBG = bg;
        Render();
    }
    void Terminal::Render()
    {
        System::Kernel::VESA.Update();
    }
    void Terminal::Write(const char* str)
    {
        //Write line using System::Kernel::VESA::DrawChar
        for (uint32_t i = 0; i < strlen(str); i++)
        {
            if (str[i] == '\n')
            {
                cursorX = 0;
                cursorY++;
            }
            else
            {
                System::Kernel::VESA.DrawChar(cursorX * 8, cursorY * 16, str[i], Color::White, Color::Black,System::HAL::Video::Fonts::System8x16);
                cursorX++;
            }
        }
    }
    void Terminal::Write(Color fg,const char* str)
    {
        //Write line using System::Kernel::VESA::DrawChar
        Color OldCol = GetFG();
        CurCol = fg;
        for (uint32_t i = 0; i < strlen(str); i++)
        {
            if (str[i] == '\n')
            {
                cursorX = 0;
                cursorY++;
            }
            else
            {
                System::Kernel::VESA.DrawChar(cursorX * 8, cursorY * 16, str[i], fg, Color::Black,System::HAL::Video::Fonts::System8x16);
                cursorX++;
            }
        }
        CurCol = OldCol;
    }
    void Terminal::Write(Color fg,Color bg,const char* str)
    {
        Color OldFGCol = GetFG();
        Color OldBGCol = GetBG();
        CurCol = fg;
        CurBG = bg;
        //Write line using System::Kernel::VESA::DrawChar
        for (uint32_t i = 0; i < strlen(str); i++)
        {
            if (str[i] == '\n')
            {
                cursorX = 0;
                cursorY++;
            }
            else
            {
                System::Kernel::VESA.DrawChar(cursorX * 8, cursorY * 16, str[i], fg, bg,System::HAL::Video::Fonts::System8x16);
                cursorX++;
            }
        }
        CurCol = OldFGCol;
        CurBG = OldBGCol;
    }
    void Terminal::WriteChar(char c)
    {
        if (c == '\n')
        {
            cursorX = 0;
            cursorY++;
        }
        else
        {
            System::Kernel::VESA.DrawChar(cursorX * 8, cursorY * 16, c, CurCol, CurBG,System::HAL::Video::Fonts::System8x16);
            cursorX++;
        }
    }
        void Terminal::WriteChar(Color FG,Color BG,char c)
    {
        if (c == '\n')
        {
            cursorX = 0;
            cursorY++;
        }
        else
        {
            System::Kernel::VESA.DrawChar(cursorX * 8, cursorY * 16, c, FG, BG,System::HAL::Video::Fonts::System8x16);
            cursorX++;
        }
    }
    void Terminal::Printf(const char* str, ...)
    {
        va_list args;
        va_start(args, str);
        //We malloc the buffer
        char* buffer = (char*)tmalloc(1024,ALLOCTYPE_STRING);
        term_vsprintf(str, args);
        va_end(args);
        Write(buffer);
        free(buffer);
    }
    void Terminal::Printf(Color FG,Color BG,const char* str, ...)
    {
        va_list args;
        va_start(args, str);
        //We malloc the buffer
        char* buffer = (char*)tmalloc(1024,ALLOCTYPE_STRING);
        term_vsprintf_color(FG,BG,str, args);
        va_end(args);
        Write(FG,BG,buffer);
        free(buffer);
    }
    void Terminal::Clear()
    {
        cursorX,cursorY = 0;
        //Clear screen using System::Kernel::VESA::Clear
        System::Kernel::VESA.Clear((uint32_t)Color::Black);
    }
}