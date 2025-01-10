#include "displayBuffer.h"
#include <cstdlib>
#include <cstring>

displayBuffer::displayBuffer(uint16_t width, uint16_t height, uint16_t xShift, uint16_t yShift, uint8_t bitsPerPixel)
{
    mWidth = width;
    mHeight = height;
    mXShift = xShift;
    mYShift = yShift;
    mBitsPerPixel = bitsPerPixel;
    mBufferSize = (mWidth * mHeight * mBitsPerPixel) >> 3;
    mBuffer = static_cast<uint8_t*>(malloc(mBufferSize));
    mRotation = 0;
    memset(mBuffer, 0, mBufferSize);
}

displayBuffer::~displayBuffer()
{
    free(mBuffer);
}

uint16_t displayBuffer::getWidth()
{
    return (mRotation == 90 || mRotation == 270) ? mHeight : mWidth;
}

uint16_t displayBuffer::getHeight()
{
    return (mRotation == 90 || mRotation == 270) ? mWidth : mHeight;
}

uint16_t displayBuffer::getXShift()
{
    return (mRotation == 90 || mRotation == 270) ? mYShift : mXShift;
}

uint16_t displayBuffer::getYShift()
{
    return (mRotation == 90 || mRotation == 270) ? mXShift : mYShift;
}

uint8_t displayBuffer::getBitsPerPixel()
{
    return mBitsPerPixel;
}

uint32_t displayBuffer::getBufferSize()
{
    return mBufferSize;
}

uint8_t* displayBuffer::getBuffer()
{
    return mBuffer;
}

uint16_t displayBuffer::getRotation()
{
    return mRotation;
}

void displayBuffer::setRotation(uint16_t degrees)
{
    mRotation = degrees;
}