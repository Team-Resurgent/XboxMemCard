#pragma once

#include "pico/stdlib.h"

class color
{
public:
    static uint8_t convertR8G8B8toGray8(uint32_t colorR8G8B8);
    static uint8_t convertR8G8B8toGray4(uint32_t colorR8G8B8);
    static uint8_t convertR8G8B8toGray1(uint32_t colorR8G8B8);
    static uint16_t convertR8G8B8toR5G6B5(uint32_t colorR8G8B8);

    static uint32_t convertGray8toR8G8B8(uint8_t gray8);
    static uint32_t convertGray4toR8G8B8(uint8_t gray4);
    static uint32_t convertGray1toR8G8B8(uint8_t gray1);
    static uint32_t convertR5G6B5toR8G8B8(uint16_t colorR5G6B5);
};