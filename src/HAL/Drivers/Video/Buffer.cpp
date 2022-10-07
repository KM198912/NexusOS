#include <HAL/Drivers/Video/Buffer.hpp>
#include <core/main.hpp>
namespace System
{
    namespace Video
    {
        
        Buffer::buf Buffer::Initialize(uint32_t* buffer, uint32_t width, uint32_t height, uint32_t pitch, uint32_t size)
        {
            Buffer::buf bug;
            bug.buffer = (uint32_t*)System::Kernel::MM.Allocate(width*height*4);
            bug.width = width;
            bug.height = height;
            bug.pitch = pitch;
            bug.size = size;
            return bug;
        }
    }
}