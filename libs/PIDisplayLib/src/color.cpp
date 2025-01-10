#include "color.h"

uint8_t color::convertR8G8B8toGray8(uint32_t colorR8G8B8) 
{
    uint8_t r8 = (colorR8G8B8 >> 16) & 0xff;
    uint8_t g8 = (colorR8G8B8 >> 8) & 0xff;
    uint8_t b8 = colorR8G8B8 & 0xff;
    return static_cast<uint8_t>(0.2126 * r8 + 0.7152 * g8 + 0.0722 * b8);
}

uint8_t color::convertR8G8B8toGray4(uint32_t colorR8G8B8) 
{
    return convertR8G8B8toGray8(colorR8G8B8) >> 4;
}

uint8_t color::convertR8G8B8toGray1(uint32_t colorR8G8B8)
{
    return convertR8G8B8toGray8(colorR8G8B8) >> 7;
}

uint16_t color::convertR8G8B8toR5G6B5(uint32_t colorR8G8B8)
{
    uint16_t r8 = (colorR8G8B8 >> 16) & 0xff;
    uint16_t g8 = (colorR8G8B8 >> 8) & 0xff;
    uint16_t b8 = colorR8G8B8 & 0xff;
    return ((r8 >> 3) << 11) | ((g8 >> 2) << 5) | (b8 >> 3);
}

uint32_t color::convertGray8toR8G8B8(uint8_t gray8) 
{
    uint32_t r8 = gray8;
    uint32_t g8 = gray8;
    uint32_t b8 = gray8;
    return (r8 << 16) | (g8 << 8) | b8;
}

uint32_t color::convertGray4toR8G8B8(uint8_t gray4) 
{
    return convertGray8toR8G8B8((gray4 & 0xf) << 4);
}

uint32_t color::convertGray1toR8G8B8(uint8_t gray1) 
{
    return convertGray8toR8G8B8((gray1 & 0x1) << 7);
}

uint32_t color::convertR5G6B5toR8G8B8(uint16_t colorR5G6B5)
{
    uint32_t r5 = ((colorR5G6B5 >> 11) & 0x1f);
    uint32_t g6 = ((colorR5G6B5 >> 5) & 0x3f);
    uint32_t b5 = (colorR5G6B5 & 0x1f);
    return ((r5 << 3) << 16) | ((g6 << 2) << 8) | (b5 << 3);
}