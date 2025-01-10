#include "displaySH1122.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define SH1122_CMD_SET_LOW_COLUMN_ADDR 0x00
#define SH1122_CMD_SET_HIGH_COLUMN_ADDR 0x10
#define SH1122_CMD_SET_DISPLAY_START_LINE 0x40 
#define SH1122_CMD_SET_CONTRAST_CURRENT 0x81 // 2 Byte Command
#define SH1122_CMD_SET_SEGMENT_REMAP 0xA0
#define SH1122_CMD_SET_ENTIRE_DISPLAY_OFF 0xA4
#define SH1122_CMD_SET_ENTIRE_DISPLAY_ON 0xA5
#define SH1122_CMD_SET_NORMAL_DISPLAY 0xA6
#define SH1122_CMD_SET_REVERSE_DISPLAY 0xA7
#define SH1122_CMD_SET_MULTIPLEX_RATIO 0xA8 // 2 Byte Command
#define SH1122_CMD_SET_DCDC_SETTING 0xAD // 2 Byte Command
#define SH1122_CMD_SET_DISPLAY_OFF 0xAE
#define SH1122_CMD_SET_DISPLAY_ON 0xAF
#define SH1122_CMD_SET_ROW_ADDR 0xB0 // 2 Byte Command
#define SH1122_CMD_SET_SCAN_DIRECTION 0xC0
#define SH1122_CMD_SET_DISPLAY_OFFSET 0xD3 // 2 Byte Command
#define SH1122_CMD_SET_CLOCK_DIVIDER 0xD5 // 2 Byte Command
#define SH1122_CMD_SET_DISCHARGE_PRECHARGE_PERIOD 0xD9 // 2 Byte Command
#define SH1122_CMD_SET_VCOM_DESELECT_LEVEL 0xDB // 2 Byte Command
#define SH1122_CMD_SET_VSEGM_LEVEL 0xDC // 2 Byte Command
#define SH1122_CMD_SET_DISCHARGE_VSL_LEVEL 0x30
#define SH1122_CMD_SET_READ_MODIFY_WRITE 0xE0
#define SH1122_CMD_SET_END 0xEE
#define SH1122_CMD_SET_NOP 0xE3

displaySH1122::displaySH1122()
{
    initDisplayBuffer(
        DISPLAY_SH1122_WIDTH, 
        DISPLAY_SH1122_HEIGHT, 
        0,
        0,
        DISPLAY_SH1122_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_SH1122_SPI, DISPLAY_SH1122_BAUDRATE);

    writeCommandByte(SH1122_CMD_SET_DISPLAY_OFF);
    writeCommandByte(SH1122_CMD_SET_ENTIRE_DISPLAY_OFF);
    writeCommandByte(SH1122_CMD_SET_DISPLAY_START_LINE | 0);

    uint8_t contrastData[] = {SH1122_CMD_SET_CONTRAST_CURRENT, 0x80};
    writeCommand(contrastData, sizeof(contrastData));

    writeCommandByte(SH1122_CMD_SET_SEGMENT_REMAP | 0x00);
    writeCommandByte(SH1122_CMD_SET_NORMAL_DISPLAY);

    uint8_t multiplexData[] = {SH1122_CMD_SET_MULTIPLEX_RATIO, DISPLAY_SH1122_HEIGHT - 1};
    writeCommand(multiplexData, sizeof(multiplexData));

    uint8_t dcdcData[] = {SH1122_CMD_SET_DCDC_SETTING, 0x81};
    writeCommand(dcdcData, sizeof(dcdcData));

    writeCommandByte(SH1122_CMD_SET_SCAN_DIRECTION | 0x00);

    uint8_t displayOffetData[] = {SH1122_CMD_SET_DISPLAY_OFFSET, 0x00};
    writeCommand(displayOffetData, sizeof(displayOffetData));

    uint8_t clockDividerData[] = {SH1122_CMD_SET_CLOCK_DIVIDER, 0x50};
    writeCommand(clockDividerData, sizeof(clockDividerData));

    uint8_t dischargePeriodData[] = {SH1122_CMD_SET_DISCHARGE_PRECHARGE_PERIOD, 0x22};
    writeCommand(dischargePeriodData, sizeof(dischargePeriodData));

    uint8_t vcomDeselectData[] = {SH1122_CMD_SET_VCOM_DESELECT_LEVEL, 0x35};
    writeCommand(vcomDeselectData, sizeof(vcomDeselectData));

    uint8_t vsgemData[] = {SH1122_CMD_SET_VSEGM_LEVEL, 0x35};
    writeCommand(vsgemData, sizeof(vsgemData));

    writeCommandByte(SH1122_CMD_SET_DISCHARGE_VSL_LEVEL | 0x00);

    uint8_t rowAddrData[] = {SH1122_CMD_SET_ROW_ADDR, 0x00};
    writeCommand(rowAddrData, sizeof(rowAddrData));

    writeCommandByte(SH1122_CMD_SET_LOW_COLUMN_ADDR);
    writeCommandByte(SH1122_CMD_SET_HIGH_COLUMN_ADDR);
    writeCommandByte(SH1122_CMD_SET_DISPLAY_ON);

    drawDisplay();
}

void displaySH1122::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void displaySH1122::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void displaySH1122::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void displaySH1122::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void displaySH1122::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displaySH1122::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void displaySH1122::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void displaySH1122::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displaySH1122::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void displaySH1122::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
{
    if (x >= mDisplayBuffer->getWidth() || y >= mDisplayBuffer->getHeight())
    {
        return;
    }

    uint8_t gray4 = color::convertR8G8B8toGray4(colorR8G8B8);
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pixelOffset = (y * (mDisplayBuffer->getWidth() >> 1)) + (x >> 1);
    uint8_t currentPixel = buffer[pixelOffset];
    if ((x & 1) == 0) 
    {
        currentPixel &= 0x0f;
        currentPixel |= (gray4 << 4);
    }
    else
    {
        currentPixel &= 0xf0;
        currentPixel |= gray4;
    }
    buffer[pixelOffset] = currentPixel;
}

void displaySH1122::fill(uint32_t colorR8G8B8)
{
    uint8_t gray4 = color::convertR8G8B8toGray4(colorR8G8B8);
    uint8_t value = (gray4 << 4) | gray4;
    memset(getDisplayBuffer()->getBuffer(), value, getDisplayBuffer()->getBufferSize());
}

void displaySH1122::drawDisplay()
{
    uint8_t rowAddrData[] = {SH1122_CMD_SET_ROW_ADDR, 0x00};
    writeCommand(rowAddrData, sizeof(rowAddrData));

    writeCommandByte(SH1122_CMD_SET_LOW_COLUMN_ADDR);
    writeCommandByte(SH1122_CMD_SET_HIGH_COLUMN_ADDR);
    writeData(getDisplayBuffer()->getBuffer(), getDisplayBuffer()->getBufferSize());
}

void displaySH1122::brightness(uint8_t value)
{
    // NA
}

void displaySH1122::contrast(uint8_t value)
{
    uint8_t contrastData[] = {SH1122_CMD_SET_CONTRAST_CURRENT, value};
    writeCommand(contrastData, sizeof(contrastData));
}

void displaySH1122::invert(bool value)
{
    writeCommandByte(value ? SH1122_CMD_SET_REVERSE_DISPLAY : SH1122_CMD_SET_NORMAL_DISPLAY);
}

void displaySH1122::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeCommandByte(SH1122_CMD_SET_DISPLAY_START_LINE | 0x00);
	    writeCommandByte(SH1122_CMD_SET_SEGMENT_REMAP | 0x00);
	    writeCommandByte(SH1122_CMD_SET_SCAN_DIRECTION | 0x00);
    }
    else if (degrees == 180)
    {
        writeCommandByte(SH1122_CMD_SET_DISPLAY_START_LINE | (uint8_t)mDisplayBuffer->getWidth());
	    writeCommandByte(SH1122_CMD_SET_SEGMENT_REMAP | 0x01);
	    writeCommandByte(SH1122_CMD_SET_SCAN_DIRECTION | 0x08);
    }
}