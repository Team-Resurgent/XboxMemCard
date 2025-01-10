#pragma once

#include "pico/stdlib.h"

class displayBuffer
{
public:
    displayBuffer(uint16_t width, uint16_t height, uint16_t xShift, uint16_t yShift, uint8_t bitsPerPixel);
    ~displayBuffer();
    uint16_t getWidth();
    uint16_t getHeight();
    uint16_t getXShift();
    uint16_t getYShift();
    uint8_t getBitsPerPixel();
    uint32_t getBufferSize();
    uint8_t* getBuffer();
    uint16_t getRotation();
    void setRotation(uint16_t degrees);
private:
    uint16_t mWidth;
    uint16_t mHeight;
    uint16_t mXShift;
    uint16_t mYShift;
    uint8_t mBitsPerPixel;
    uint32_t mBufferSize;
    uint8_t* mBuffer;
    uint16_t mRotation;
};