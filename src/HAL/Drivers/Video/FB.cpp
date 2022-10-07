#include <HAL/Drivers/Video/FB.hpp>
#include <core/main.hpp>
namespace System
{
    namespace HAL
    {
        namespace Video
        {
            void VESA::Initialize()
            {
                buffer = (uint32_t*)System::Kernel::manager.Parse()->framebuffer_addr;
                width = System::Kernel::manager.Parse()->framebuffer_width;
                height = System::Kernel::manager.Parse()->framebuffer_height;
                pitch = System::Kernel::manager.Parse()->framebuffer_pitch;
                size = width * height * 4;
            }
            void VESA::SetPixel(uint32_t x, uint32_t y, uint32_t color)
            {
               //Set pixel using pitch
                buffer[y * (pitch / 4) + x] = color;
            }
            void VESA::Clear()
            {
                for (uint32_t i = 0; i < size; i++)
                {
                    buffer[i] = 0x00000000;
                }
            }
            void VESA::Clear(uint32_t color)
            {
                for (uint32_t i = 0; i < size; i++)
                {
                    buffer[i] = color;
                }
            }
        }
    }
}