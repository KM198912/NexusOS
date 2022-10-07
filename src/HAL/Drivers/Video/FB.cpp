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
                buffer = (uint32_t*)System::Kernel::manager.Parse()->framebuffer_addr;
                width = System::Kernel::manager.Parse()->framebuffer_width;
                height = System::Kernel::manager.Parse()->framebuffer_height;
                pitch = System::Kernel::manager.Parse()->framebuffer_pitch;
                size = width * height * 4;
                bugs = bb.Initialize(buffer, width, height, pitch, size);
            }
            void VESA::SetPixel(uint32_t x, uint32_t y, uint32_t color)
            {
               //Set pixel using pitch
                bugs.buffer[y * (bugs.pitch / 4) + x] = color;
            }
            void VESA::Clear()
            {
                for (uint32_t i = 0; i < size; i++)
                {
                    bugs.buffer[i] = 0x00000000;
                }
            }
            void VESA::Clear(uint32_t color)
            {
                for (uint32_t i = 0; i < size; i++)
                {
                    bugs.buffer[i] = color;
                }
            }
            void VESA::Update()
            {
                for (uint32_t i = 0; i < size; i++)
                {
                    buffer[i] = bugs.buffer[i];
                }
            }
        }
    }
}