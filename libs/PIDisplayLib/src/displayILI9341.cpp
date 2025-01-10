#include "displayILI9341.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define ILI9341_NOP 0x00
#define ILI9341_SOFTWARE_RESET 0x01
#define ILI9341_SLEEP_IN 0x10
#define ILI9341_SLEEP_OUT 0x11
#define ILI9341_PARTIAL_MODE_ON 0x12
#define ILI9341_PARTIAL_MODE_OFF 0x13
#define ILI9341_DISPLAY_INVERSION_OFF 0x20
#define ILI9341_DISPLAY_INVERSION_ON 0x21
#define ILI9341_GAMMA_SET 0x26 // 2 Byte Command
#define ILI9341_DISPLAY_OFF 0x28
#define ILI9341_DISPLAY_ON 0x29
#define ILI9341_COLUMN_ADDRESS_SET 0x2A // 5 Byte Command
#define ILI9341_PAGE_ADDRESS_SET 0x2B // 5 Byte Command
#define ILI9341_MEMORY_WRITE 0x2C // 2+ Byte Command
#define ILI9341_COLOR_SET 0x2D // 10 Byte Command
#define ILI9341_PARTIAL_START_END 0x30 // 5 Byte Command
#define ILI9341_VERTICAL_SCROLLING_DIRECTION 0x33 // 7 Byte Command
#define ILI9341_TEARING_EFFECT_LINE_OFF 0x34
#define ILI9341_TEARING_EFFECT_LINE_ON 0x35
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL 0x36 // 2 Byte Command
#define ILI9341_VERTICAL_SCROLLING_START_ADDRESS 0x37 // 3 Byte Command
#define ILI9341_IDLE_MODE_OFF 0x38
#define ILI9341_IDLE_MODE_ON 0x39
#define ILI9341_INTERFACE_PIXEL_FORMAT 0x3A // 2 Byte Command
#define ILI9341_MEMORY_WRITE_CONTINUE 0x3C // 2+ Byte Command
#define ILI9341_SET_TEAR_SCANLINE 0x44 // 3 Byte Command
#define ILI9341_WRITE_DISPLAY_BRIGHTNESS 0x51 // 2 Byte Command
#define ILI9341_WRITE_CTRL_DISPLAY 0x53 // 2 Byte Command
#define ILI9341_CONTENT_ADAPTIVE_BRIGHTNESS 0x55 // 2 Byte Command
#define ILI9341_WRITE_CABC_MINIMUM_BRIGHTNESS 0x5E // 2 Byte Command

#define ILI9341_RGB_CONTROL 0xB0 // 2 Byte Command
#define ILI9341_FRAME_CONTROL_NORMAL 0xB1 // 2 Byte Command
#define ILI9341_FRAME_CONTROL_IDLE 0xB2 // 2 Byte Command
#define ILI9341_FRAME_CONTROL_PARTIAL 0xB3 // 2 Byte Command
#define ILI9341_DISPLAY_INVERSION_CONTROL 0xB4 // 2 Byte Command
#define ILI9341_BLANKING_PORCH_CONTROL 0xB5 // 5 Byte Command
#define ILI9341_DISPLAY_FUNCTION_CONTROL 0xB6 // 5 Byte Command
#define ILI9341_ENTRY_MODE_SET 0xB7 // 2 Byte Command
#define ILI9341_BACKLIGHT_CONTROL1 0xB8 // 3 Byte Command
#define ILI9341_BACKLIGHT_CONTROL2 0xB9 // 3 Byte Command
#define ILI9341_BACKLIGHT_CONTROL3 0xBA // 3 Byte Command
#define ILI9341_BACKLIGHT_CONTROL4 0xBB // 3 Byte Command
#define ILI9341_BACKLIGHT_CONTROL5 0xBC // 3 Byte Command
#define ILI9341_BACKLIGHT_CONTROL7 0xBE // 2 Byte Command
#define ILI9341_BACKLIGHT_CONTROL8 0xBF // 2 Byte Command
#define ILI9341_POWER_CONTROL1 0xC0 // 2 Byte Command
#define ILI9341_POWER_CONTROL2 0xC1 // 2 Byte Command
#define ILI9341_VCOM_CONTROL1 0xC5 // 3 Byte Command
#define ILI9341_VCOM_CONTROL2 0xC7 // 2 Byte Command
#define ILI9341_NV_MEMORY_WRITE 0xD0 // 3 Byte Command
#define ILI9341_NV_MEMORY_PROTECTION_KEY 0xD1 // 4 Byte Command
#define ILI9341_POSITIVE_VOLTAGE_GAMMA_CONTROL 0xE0 // 16 Byte Command
#define ILI9341_NEGATIVE_VOLTAGE_GAMMA_CONTROL 0xE1 // 16 Byte Command
#define ILI9341_DIGITAL_GAMMA_CONTROL1 0xE2 // 17 Byte Command
#define ILI9341_DIGITAL_GAMMA_CONTROL2 0xE3 // 65 Byte Command
#define ILI9341_INTERFACE_CONTROL 0xF6 // 4 Byte Command

#define ILI9341_INTERFACE_PIXEL_FORMAT_16BIT 0x05
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MY 0x80
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MX 0x40
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MV 0x20
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL_ML 0x10
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL_BGR 0x00
#define ILI9341_MEMORY_ADDRESS_DATA_CONTROL_RGB 0x08

displayILI9341::displayILI9341()
{
    initDisplayBuffer(
        DISPLAY_ILI9341_WIDTH, 
        DISPLAY_ILI9341_HEIGHT, 
        DISPLAY_ILI9341_X_SHIFT,
        DISPLAY_ILI9341_Y_SHIFT,
        DISPLAY_ILI9341_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_ILI9341_SPI, DISPLAY_ILI9341_BAUDRATE);

    writeCommandByte(ILI9341_SOFTWARE_RESET);
    sleep_ms(100);

    writeCommandByte(0xea);
    uint8_t unknown6[] = {0x00, 0x00};
    writeData(unknown6, sizeof(unknown6));   
    
    writeCommandByte(ILI9341_POWER_CONTROL1);
    writeDataByte(0x23);   

    writeCommandByte(ILI9341_POWER_CONTROL2);
    writeDataByte(0x10);  

    writeCommandByte(ILI9341_VCOM_CONTROL1);
    uint8_t vcomControl1[] = {0x3e, 0x28};
    writeData(vcomControl1, sizeof(vcomControl1)); 

    writeCommandByte(ILI9341_VCOM_CONTROL2);
    writeDataByte(0x86);  

    writeCommandByte(ILI9341_VERTICAL_SCROLLING_START_ADDRESS);
    writeDataByte(0x00);  

    writeCommandByte(ILI9341_INTERFACE_PIXEL_FORMAT);
    writeDataByte(ILI9341_INTERFACE_PIXEL_FORMAT_16BIT);  

    writeCommandByte(ILI9341_FRAME_CONTROL_NORMAL);
    uint8_t frameRateControl[] = {0x00, 0x18};
	writeData(frameRateControl, sizeof(frameRateControl));

    writeCommandByte(ILI9341_DISPLAY_FUNCTION_CONTROL);
    uint8_t displayFunction[] = {0x08, 0x82, 0x27};
	writeData(displayFunction, sizeof(displayFunction));

    writeCommandByte(ILI9341_GAMMA_SET);
    writeDataByte(0x01);  

    writeCommandByte(ILI9341_POSITIVE_VOLTAGE_GAMMA_CONTROL);
    uint8_t positiveGamma[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    writeData(positiveGamma, sizeof(positiveGamma));

    writeCommandByte(ILI9341_NEGATIVE_VOLTAGE_GAMMA_CONTROL);
    uint8_t negativeGamma[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
    writeData(negativeGamma, sizeof(negativeGamma));

    writeCommandByte(ILI9341_DISPLAY_INVERSION_OFF);
	writeCommandByte(ILI9341_SLEEP_OUT);
  	writeCommandByte(ILI9341_PARTIAL_MODE_OFF);
  	writeCommandByte(ILI9341_DISPLAY_ON);

    rotate(0);

    drawDisplay();
}

void displayILI9341::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void displayILI9341::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void displayILI9341::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void displayILI9341::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void displayILI9341::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayILI9341::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void displayILI9341::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void displayILI9341::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayILI9341::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void displayILI9341::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
{
    if (x >= mDisplayBuffer->getWidth() || y >= mDisplayBuffer->getHeight())
    {
        return;
    }

    uint16_t r5g6b5 = color::convertR8G8B8toR5G6B5(colorR8G8B8);
    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    uint32_t pixelOffset = (y * (mDisplayBuffer->getWidth() << 1)) + (x << 1);
    buffer[pixelOffset] = static_cast<uint8_t>((r5g6b5 & 0xff00) >> 8);
    buffer[pixelOffset + 1] = static_cast<uint8_t>(r5g6b5 & 0xff);
}

void displayILI9341::fill(uint32_t colorR8G8B8)
{
    uint16_t r5g6b5 = color::convertR8G8B8toR5G6B5(colorR8G8B8);

    uint32_t rowStride = mDisplayBuffer->getWidth() << 1;
    uint8_t* rowData = (uint8_t*)malloc(rowStride);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < mDisplayBuffer->getWidth(); i++)
    {
        rowData[offset] = static_cast<uint8_t>(r5g6b5 >> 8);
        rowData[offset + 1] = static_cast<uint8_t>(r5g6b5 & 0xff);
        offset += 2;
    }

    uint8_t* buffer = getDisplayBuffer()->getBuffer();
    for (uint32_t i = 0; i < mDisplayBuffer->getHeight(); i++) 
    {
        memcpy(buffer, rowData, rowStride);
        buffer += rowStride;
    }
}

void displayILI9341::drawDisplay()
{
	uint16_t xStart = 0 + mDisplayBuffer->getXShift();
    uint16_t xEnd = mDisplayBuffer->getWidth() + mDisplayBuffer->getXShift() - 1;
	uint16_t yStart = 0 + mDisplayBuffer->getYShift();
    uint16_t yEnd = mDisplayBuffer->getHeight() + mDisplayBuffer->getYShift() - 1;

	writeCommandByte(ILI9341_COLUMN_ADDRESS_SET);
    uint8_t columnData[] = {(uint8_t)(xStart >> 8), (uint8_t)(xStart & 0xFF), (uint8_t)(xEnd >> 8), (uint8_t)(xEnd & 0xFF)};
    writeData(columnData, sizeof(columnData));

	writeCommandByte(ILI9341_PAGE_ADDRESS_SET);
    uint8_t rowData[] = {(uint8_t)(yStart >> 8), (uint8_t)(yStart & 0xFF), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xFF)};
    writeData(rowData, sizeof(rowData));

	writeCommandByte(ILI9341_MEMORY_WRITE);
    writeData(getDisplayBuffer()->getBuffer(), getDisplayBuffer()->getBufferSize());
}

void displayILI9341::brightness(uint8_t value)
{
    // Does not seem to work
    // writeCommandByte(ILI9341_WRITE_DISPLAY_BRIGHTNESS);
    // writeDataByte(value);
}

void displayILI9341::contrast(uint8_t value)
{
    // NA 
}

void displayILI9341::invert(bool value)
{
    writeCommandByte(value ? ILI9341_DISPLAY_INVERSION_OFF : ILI9341_DISPLAY_INVERSION_ON);
}

void displayILI9341::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeCommandByte(ILI9341_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte(ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MY |  
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_RGB);
    }
    else if (degrees == 90)
    {
        writeCommandByte(ILI9341_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte((uint8_t)(
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 180)
    {
        writeCommandByte(ILI9341_MEMORY_ADDRESS_DATA_CONTROL);	
	    writeDataByte((uint8_t)(
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 270)
    {
        writeCommandByte(ILI9341_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte((uint8_t)(
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            ILI9341_MEMORY_ADDRESS_DATA_CONTROL_RGB));
    }
}