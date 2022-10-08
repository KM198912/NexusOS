#pragma once
#include <stdint.h>
#include <lib/fontdata.hpp>

namespace System
{
    namespace HAL
    {
        namespace Video
        {
    typedef struct
	{
		uint8_t  Width;
		uint8_t  Height;
		uint8_t  SpacingX;
		uint8_t  SpacingY;
		uint8_t* Data;
	} __attribute__((packed)) Font;

    namespace Fonts
    {
		static const Font System8x16 = { 8, 16, 0, 0, (uint8_t*)Video::Font_System8x16_Data };
		static const Font System8x8  = { 8, 8,  0, 0, (uint8_t*)Video::Font_System8x8_Data  };
		static const Font Serif8x16  = { 8, 16, 0, 0, (uint8_t*)Video::Font_Serif8x16_Data  };
		static const Font Serif8x8   = { 8, 8,  0, 0, (uint8_t*)Video::Font_Serif8x8_Data   };
    }
}
    }
    }

static inline bool GetBitAddressFromByte(uint8_t to_convert, int to_return)
{
    int mask = 1 << (to_return - 1);
    return ((to_convert & mask) != 0);
}