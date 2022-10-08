#include <HAL/Drivers/Video/FB.hpp>
#include <core/main.hpp>
#include <HAL/Drivers/Video/Buffer.hpp>
namespace System
{
    namespace HAL
    {
        namespace Video
        {
            System::Video::Buffer::buf bugs;
            System::Video::Buffer bb;
            void VESA::Initialize()
            {
               // System::Kernel::paging.Map(0x8000000,System::Kernel::manager.Parse()->framebuffer_addr);
                buffer = (uint32_t*)System::Kernel::manager.Parse()->framebuffer_addr;
                memset(buffer,0,System::Kernel::manager.Parse()->framebuffer_pitch * System::Kernel::manager.Parse()->framebuffer_height);
                printf("%s Found VBE Physical Base at: %x\n",DEBUG_INFO,buffer);
                width = System::Kernel::manager.Parse()->framebuffer_width;
                printf("%s VBE Width Set to: %d\n",DEBUG_INFO,width);
                height = System::Kernel::manager.Parse()->framebuffer_height;
                pitch = System::Kernel::manager.Parse()->framebuffer_pitch;
                size = width * height * 4;
                bugs = bb.Initialize(buffer, width, height, pitch, size);
            }
            void VESA::SetPixel(uint32_t x, uint32_t y, uint32_t color)
            {
               //Set pixel using pitch
                bugs.buffer[y * (pitch / 4) + x] = color;
            }
            void VESA::Clear()
            {
               for(uint32_t i = 0; i < size; i++)
               {
                   bugs.buffer[i] = (uint32_t)Color::Black;
               }
            }
            void VESA::Clear(uint32_t color)
            {
               for(uint32_t i = 0; i < size; i++)
               {
                   bugs.buffer[i] = color;
               }
            }
            void VESA::Clear(Color color)
            {
                for(uint32_t i = 0; i < size; i++)
               {
                   bugs.buffer[i] = (uint32_t)color;
               }
            }
            void VESA::DrawFilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
            {
                for (uint32_t i = 0; i < h; i++)
                {
                    for (uint32_t j = 0; j < w; j++)
                    {
                        SetPixel(x + j, y + i, color);
                    }
                }
            }
            void VESA::DrawOutlineRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
            {
                for (uint32_t i = 0; i < h; i++)
                {
                    for (uint32_t j = 0; j < w; j++)
                    {
                        if (i == 0 || i == h - 1 || j == 0 || j == w - 1)
                        {
                            SetPixel(x + j, y + i, color);
                        }
                    }
                }
            }
            uint32_t VESA::GetWidth()
            {
                return width;
            }
            uint32_t VESA::GetHeight()
            {
                return height;
            }
            void VESA::DrawChar(uint16_t x, uint16_t y, char c, Color fg, Color bg, Font font)
        {
            DrawFilledRect(x, y, font.Width + font.SpacingX, font.Height + font.SpacingY, (uint32_t)bg);
            uint32_t p = font.Height * c;
            for (size_t cy = 0; cy < font.Height; cy++)
            {
                for (size_t cx = 0; cx < font.Width; cx++)
                {
                    uint32_t xx = x + (font.Width - cx);
                    uint32_t yy = y + cy;
                    if (GetBitAddressFromByte(font.Data[p + cy], cx + 1)) { SetPixel(xx, yy, (uint32_t)fg); }
                }
            }
        }
                void VESA::DrawString(uint16_t x, uint16_t y, const char* str, Color fg, Color bg, Font font)
        {
            int i = 0, xx = x, yy = y;
            while (str[i] != 0)
            {
                if (str[i] == '\n') { xx = x; yy += font.Height + font.SpacingY; }
                else if (isprint(str[i]))
                {
                    DrawChar(xx, yy, str[i], fg, bg, font);
                    xx += font.Width + font.SpacingX;
                }
                i++;
            }
        }
            void VESA::Update()
            {
                    memcpy((uint32_t*)buffer, (uint32_t*)bugs.buffer, size);
            }
        }
    }
}