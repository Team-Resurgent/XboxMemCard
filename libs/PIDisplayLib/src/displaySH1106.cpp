#include "displaySH1106.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define SH1106_CMD_SET_LOW_COLUMN_ADDR 0x00
#define SH1106_CMD_SET_HIGH_COLUMN_ADDR 0x10
#define SH1106_CMD_SET_PUMP_VOLTAGE 0x30 
#define SH1106_CMD_SET_DISPLAY_START_LINE 0x40
#define SH1106_CMD_SET_CONTRAST_CURRENT 0x81 // 2 Byte Command
#define SH1106_CMD_SET_SEGMENT_REMAP 0xA0
#define SH1106_CMD_SET_ENTIRE_DISPLAY_OFF 0xA4
#define SH1106_CMD_SET_ENTIRE_DISPLAY_ON 0xA5
#define SH1106_CMD_SET_NORMAL_DISPLAY 0xA6
#define SH1106_CMD_SET_REVERSE_DISPLAY 0xA7
#define SH1106_CMD_SET_MULTIPLEX_RATIO 0xA8 // 2 Byte Command
#define SH1106_CMD_SET_DCDC_SETTING 0xAD // 2 Byte Command
#define SH1106_CMD_SET_DISPLAY_OFF 0xAE
#define SH1106_CMD_SET_DISPLAY_ON 0xAF
#define SH1106_CMD_SET_PAGE_ADDR 0xB0
#define SH1106_CMD_SET_SCAN_DIRECTION 0xC0
#define SH1106_CMD_SET_DISPLAY_OFFSET 0xD3 // 2 Byte Command
#define SH1106_CMD_SET_CLOCK_DIVIDER 0xD5 // 2 Byte Command
#define SH1106_CMD_SET_DISCHARGE_PRECHARGE_PERIOD 0xD9 // 2 Byte Command
#define SH1106_CMD_SET_COMMON_PADS_HARDWARE 0xDA // 2 Byte Command
#define SH1106_CMD_SET_VCOM_DESELECT_LEVEL 0xDB // 2 Byte Command
#define SH1106_CMD_SET_READ_MODIFY_WRITE 0xE0
#define SH1106_CMD_SET_END 0xEE
#define SH1106_CMD_SET_NOP 0xE3

displaySH1106::displaySH1106()
{
    initDisplayBuffer(
        DISPLAY_SH1106_WIDTH, 
        DISPLAY_SH1106_HEIGHT, 
        0,
        0,
        DISPLAY_SH1106_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_SH1106_SPI, DISPLAY_SH1106_BAUDRATE);

    writeCommandByte(SH1106_CMD_SET_DISPLAY_OFF);
    writeCommandByte(SH1106_CMD_SET_ENTIRE_DISPLAY_OFF);
    writeCommandByte(SH1106_CMD_SET_DISPLAY_START_LINE | 0);

    uint8_t contrastData[] = {SH1106_CMD_SET_CONTRAST_CURRENT, 0x80};
    writeCommand(contrastData, sizeof(contrastData));

    writeCommandByte(SH1106_CMD_SET_SEGMENT_REMAP | 0x00);
    writeCommandByte(SH1106_CMD_SET_NORMAL_DISPLAY);

    uint8_t multiplexData[] = {SH1106_CMD_SET_MULTIPLEX_RATIO, DISPLAY_SH1106_HEIGHT - 1};
    writeCommand(multiplexData, sizeof(multiplexData));

    uint8_t dcdcData[] = {SH1106_CMD_SET_DCDC_SETTING, 0x81};
    writeCommand(dcdcData, sizeof(dcdcData));

    writeCommandByte(SH1106_CMD_SET_SCAN_DIRECTION | 0x00);

    uint8_t displayOffetData[] = {SH1106_CMD_SET_DISPLAY_OFFSET, 0x00};
    writeCommand(displayOffetData, sizeof(displayOffetData));

    uint8_t clockDividerData[] = {SH1106_CMD_SET_CLOCK_DIVIDER, 0x50};
    writeCommand(clockDividerData, sizeof(clockDividerData));

    uint8_t vcomDeselectData[] = {SH1106_CMD_SET_VCOM_DESELECT_LEVEL, 0x35};
    writeCommand(vcomDeselectData, sizeof(vcomDeselectData));

    writeCommandByte(SH1106_CMD_SET_PAGE_ADDR);
    writeCommandByte(SH1106_CMD_SET_LOW_COLUMN_ADDR);
    writeCommandByte(SH1106_CMD_SET_HIGH_COLUMN_ADDR);
    writeCommandByte(SH1106_CMD_SET_DISPLAY_ON);

    drawDisplay();
}

void displaySH1106::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void displaySH1106::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void displaySH1106::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void displaySH1106::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void displaySH1106::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displaySH1106::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void displaySH1106::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void displaySH1106::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displaySH1106::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void displaySH1106::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
{
    if (x >= mDisplayBuffer->getWidth() || y >= mDisplayBuffer->getHeight())
    {
        return;
    }

    uint8_t gray1 = color::convertR8G8B8toGray1(colorR8G8B8);
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pixelOffset = x + (y >> 3) * mDisplayBuffer->getWidth();
    uint8_t currentPixel = buffer[pixelOffset];
    uint8_t pixelBit = y & 0x7;
    currentPixel &= ~(1 << pixelBit);
    currentPixel |= gray1 << pixelBit;
    buffer[pixelOffset] = currentPixel;
}

void displaySH1106::fill(uint32_t colorR8G8B8)
{
    uint8_t gray1 = color::convertR8G8B8toGray1(colorR8G8B8);
    uint8_t value = 
        (gray1 << 7) | (gray1 << 6) | 
        (gray1 << 5) | (gray1 << 4) | 
        (gray1 << 3) | (gray1 << 2) | 
        (gray1 << 1) | gray1; 
    memset(getDisplayBuffer()->getBuffer(), value, getDisplayBuffer()->getBufferSize());
}

void displaySH1106::drawDisplay()
{
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pageSize = getDisplayBuffer()->getWidth();

    uint16_t pages = getDisplayBuffer()->getHeight() >> 3;
    for (uint16_t page = 0; page < pages; page++)
    {
        writeCommandByte(SH1106_CMD_SET_PAGE_ADDR | page);
        writeCommandByte(SH1106_CMD_SET_LOW_COLUMN_ADDR | 2);
        writeCommandByte(SH1106_CMD_SET_HIGH_COLUMN_ADDR);
        writeData(buffer, pageSize);
        buffer += pageSize;
    }
}

void displaySH1106::brightness(uint8_t value)
{
    // NA
}

void displaySH1106::contrast(uint8_t value)
{
    uint8_t contrastData[] = {SH1106_CMD_SET_CONTRAST_CURRENT, value};
    writeCommand(contrastData, sizeof(contrastData));
}

void displaySH1106::invert(bool value)
{
    writeCommandByte(value ? SH1106_CMD_SET_REVERSE_DISPLAY : SH1106_CMD_SET_NORMAL_DISPLAY);
}

void displaySH1106::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeCommandByte(SH1106_CMD_SET_DISPLAY_START_LINE | 0x00);
	    writeCommandByte(SH1106_CMD_SET_SEGMENT_REMAP | 0x00);
	    writeCommandByte(SH1106_CMD_SET_SCAN_DIRECTION | 0x00);
    }
    else if (degrees == 180)
    {
        writeCommandByte(SH1106_CMD_SET_DISPLAY_START_LINE | (uint8_t)mDisplayBuffer->getWidth());
	    writeCommandByte(SH1106_CMD_SET_SEGMENT_REMAP | 0x01);
	    writeCommandByte(SH1106_CMD_SET_SCAN_DIRECTION | 0x08);
    }
}