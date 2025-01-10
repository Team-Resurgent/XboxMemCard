#include "displayGC9A01A.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

//https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp

#define GC9A01A_NOP 0x00
#define GC9A01A_SOFTWARE_RESET 0x01
#define GC9A01A_SLEEP_IN 0x10
#define GC9A01A_SLEEP_OUT 0x11
#define GC9A01A_PARTIAL_MODE_ON 0x12
#define GC9A01A_PARTIAL_MODE_OFF 0x13
#define GC9A01A_DISPLAY_INVERSION_OFF 0x20
#define GC9A01A_DISPLAY_INVERSION_ON 0x21
#define GC9A01A_DISPLAY_OFF 0x28
#define GC9A01A_DISPLAY_ON 0x29
#define GC9A01A_COLUMN_ADDRESS_SET 0x2A // 5 Byte Command
#define GC9A01A_ROW_ADDRESS_SET 0x2B // 5 Byte Command
#define GC9A01A_MEMORY_WRITE 0x2C // 2+ Byte Command
#define GC9A01A_PARTIAL_START_END 0x30 // 5 Byte Command
#define GC9A01A_VERTICAL_SCROLLING_DIRECTION 0x33 // 7 Byte Command
#define GC9A01A_TEARING_EFFECT_LINE_OFF 0x34
#define GC9A01A_TEARING_EFFECT_LINE_ON 0x35 
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL 0x36 // 2 Byte Command
#define GC9A01A_VERTICAL_SCROLLING_START_ADDRESS 0x37 // 3 Byte Command
#define GC9A01A_IDLE_MODE_OFF 0x38
#define GC9A01A_IDLE_MODE_ON 0x39
#define GC9A01A_INTERFACE_PIXEL_FORMAT 0x3A // 2 Byte Command
#define GC9A01A_MEMORY_WRITE_CONTINUE 0x3C // 2+ Byte Command
#define GC9A01A_SET_TEAR_SCANLINE 0x44 // 3 Byte Command
#define GC9A01A_WRITE_DISPLAY_BRIGHTNESS 0x51 // 2 Byte Command
#define GC9A01A_WRITE_CTRL_DISPLAY 0x53 // 2 Byte Command

#define GC9A01A_RGB_CONTROL 0xB0 // 2 Byte Command
#define GC9A01A_PORCH_CONTROL 0xB5 // 4 Byte Command
#define GC9A01A_DISPLAY_FUNCTION_CONTROL 0xB6 // 3 Byte Command
#define GC9A01A_TE_CONTROL 0xBA // 2 Byte Command
#define GC9A01A_INTERFACE_CONTROL1 0xF6 // 2 Byte Command

#define GC9A01A_POWER_CRITERION_CONTROL 0xC1 // 2 Byte Command
#define GC9A01A_VCORE_VOLTAGE_CONTROL 0xA7 // 2 Byte Command
#define GC9A01A_VREG1A_VOLTAGE_CONTROL 0xC3 // 2 Byte Command
#define GC9A01A_VREG1B_VOLTAGE_CONTROL 0xC4 // 2 Byte Command
#define GC9A02A_VREG1A_VOLTAGE_CONTROL 0xC9 // 2 Byte Command
#define GC9A01A_FRAME_RATE_CONTROL 0xE8 // 3 Byte Command
#define GC9A01A_SPI2_DATA_CONTROL 0xE9 // 2 Byte Command

#define GC9A01A_CHARGE_PUMP_FREQUENT_CONTROL 0xEC // 4 Byte Command
#define GC9A01A_INNER_REGISTER_ENABLE1 0xFE // 1 Byte Command
#define GC9A01A_INNER_REGISTER_ENABLE2 0xEF // 1 Byte Command
#define GC9A01A_SET_GAMMA1 0xF0 // 7 Byte Command
#define GC9A01A_SET_GAMMA2 0xF1 // 7 Byte Command
#define GC9A01A_SET_GAMMA3 0xF2 // 7 Byte Command
#define GC9A01A_SET_GAMMA4 0xF3 // 7 Byte Command

#define GC9A01A_INTERFACE_PIXEL_FORMAT_16BIT 0x05
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MY 0x80
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MX 0x40
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MV 0x20
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_ML 0x10
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_BGR 0x08
#define GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB 0x00

displayGC9A01A::displayGC9A01A()
{
    initDisplayBuffer(
        DISPLAY_GC9A01A_WIDTH, 
        DISPLAY_GC9A01A_HEIGHT, 
        DISPLAY_GC9A01A_X_SHIFT,
        DISPLAY_GC9A01A_Y_SHIFT,
        DISPLAY_GC9A01A_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_GC9A01A_SPI, DISPLAY_GC9A01A_BAUDRATE);

    writeCommandByte(GC9A01A_SOFTWARE_RESET);
    sleep_ms(100);

    // writeCommand(GC9A01A_INTERFACE_PIXEL_FORMAT);	
    // writeDataByte(GC9A01A_INTERFACE_PIXEL_FORMAT_16BIT);

  	// writeCommand(GC9A01A_PORCH_CONTROL);	
	// uint8_t porchData[] = { 0x0c, 0x0c, 0x00, 0x33, 0x33 };
	// writeData(porchData, sizeof(porchData));

//https://github.com/Exboom/GC9A01/blob/master/Core/Src/GC9A01.c

//https://github.com/adafruit/Adafruit_GC9A01A/blob/main/Adafruit_GC9A01A.cpp

    // Unknown
    writeCommandByte(0xEB);
    writeDataByte(0x14);
    
    writeCommandByte(GC9A01A_INNER_REGISTER_ENABLE1);
    writeCommandByte(GC9A01A_INNER_REGISTER_ENABLE2);
    
    // Unknown
    writeCommandByte(0xEB);
    writeDataByte(0x14);
    
    // Unknown
    writeCommandByte(0x84);
    writeDataByte(0x40);
    
    // Unknown
    writeCommandByte(0x85);
    writeDataByte(0xFF);
    
    // Unknown
    writeCommandByte(0x86);
    writeDataByte(0xFF);
    
    // Unknown
    writeCommandByte(0x87);
    writeDataByte(0xFF);
    
    // Unknown
    writeCommandByte(0x88);
    writeDataByte(0x0A);
    
    // Unknown
    writeCommandByte(0x89);
    writeDataByte(0x21);
    
    // Unknown
    writeCommandByte(0x8A);
    writeDataByte(0x00);
    
    // Unknown
    writeCommandByte(0x8B);
    writeDataByte(0x80);
    
    // Unknown
    writeCommandByte(0x8C);
    writeDataByte(0x01);
    
    // Unknown
    writeCommandByte(0x8D);
    writeDataByte(0x01);
    
    // Unknown
    writeCommandByte(0x8E);
    writeDataByte(0xFF);
    
    // Unknown
    writeCommandByte(0x8F);
    writeDataByte(0xFF);
    
    writeCommandByte(GC9A01A_DISPLAY_FUNCTION_CONTROL);
    writeDataByte(0x00);
    writeDataByte(0x00);
    
    writeCommandByte(GC9A01A_INTERFACE_PIXEL_FORMAT); 
    writeDataByte(GC9A01A_INTERFACE_PIXEL_FORMAT_16BIT);
    
    // Unknown
    writeCommandByte(0x90);
    writeDataByte(0x08);
    writeDataByte(0x08);
    writeDataByte(0x08);
    writeDataByte(0x08);
    
    // Unknown
    writeCommandByte(0xBD);
    writeDataByte(0x06);
    
    // Unknown
    writeCommandByte(0xBC);
    writeDataByte(0x00);
    
    // Unknown
    writeCommandByte(0xFF);
    writeDataByte(0x60);
    writeDataByte(0x01);
    writeDataByte(0x04);
    
    writeCommandByte(GC9A01A_VREG1A_VOLTAGE_CONTROL);
    writeDataByte(0x13);

    writeCommandByte(GC9A01A_VREG1B_VOLTAGE_CONTROL);
    writeDataByte(0x13);
    
    writeCommandByte(GC9A02A_VREG1A_VOLTAGE_CONTROL);
    writeDataByte(0x22);
    
    // Unknown
    writeCommandByte(0xBE);
    writeDataByte(0x11);
    
    // Unknown
    writeCommandByte(0xE1);
    writeDataByte(0x10);
    writeDataByte(0x0E);
    
    // Unknown
    writeCommandByte(0xDF);
    writeDataByte(0x21);
    writeDataByte(0x0c);
    writeDataByte(0x02);
    
    writeCommandByte(GC9A01A_SET_GAMMA1);
    writeDataByte(0x45);
    writeDataByte(0x09);
    writeDataByte(0x08);
    writeDataByte(0x08);
    writeDataByte(0x26);
    writeDataByte(0x2A);
    
    writeCommandByte(GC9A01A_SET_GAMMA2);
    writeDataByte(0x43);
    writeDataByte(0x70);
    writeDataByte(0x72);
    writeDataByte(0x36);
    writeDataByte(0x37);
    writeDataByte(0x6F);
    
    writeCommandByte(GC9A01A_SET_GAMMA3);
    writeDataByte(0x45);
    writeDataByte(0x09);
    writeDataByte(0x08);
    writeDataByte(0x08);
    writeDataByte(0x26);
    writeDataByte(0x2A);
    
    writeCommandByte(GC9A01A_SET_GAMMA4);
    writeDataByte(0x43);
    writeDataByte(0x70);
    writeDataByte(0x72);
    writeDataByte(0x36);
    writeDataByte(0x37);
    writeDataByte(0x6F);
    
    // Unknown
    writeCommandByte(0xED);
    writeDataByte(0x1B);
    writeDataByte(0x0B);
    
    // Unknown
    writeCommandByte(0xAE);
    writeDataByte(0x77);
    
    // Unknown
    writeCommandByte(0xCD);
    writeDataByte(0x63);
    
    // Unknown
    writeCommandByte(0x70);
    writeDataByte(0x07);
    writeDataByte(0x07);
    writeDataByte(0x04);
    writeDataByte(0x0E);
    writeDataByte(0x0F);
    writeDataByte(0x09);
    writeDataByte(0x07);
    writeDataByte(0x08);
    writeDataByte(0x03);
    
    writeCommandByte(GC9A01A_FRAME_RATE_CONTROL);
    writeDataByte(0x34);

    // Unknown    
    writeCommandByte(0x62);
    writeDataByte(0x18);
    writeDataByte(0x0D);
    writeDataByte(0x71);
    writeDataByte(0xED);
    writeDataByte(0x70);
    writeDataByte(0x70);
    writeDataByte(0x18);
    writeDataByte(0x0F);
    writeDataByte(0x71);
    writeDataByte(0xEF);
    writeDataByte(0x70);
    writeDataByte(0x70);
    
    // Unknown
    writeCommandByte(0x63);
    writeDataByte(0x18);
    writeDataByte(0x11);
    writeDataByte(0x71);
    writeDataByte(0xF1);
    writeDataByte(0x70);
    writeDataByte(0x70);
    writeDataByte(0x18);
    writeDataByte(0x13);
    writeDataByte(0x71);
    writeDataByte(0xF3);
    writeDataByte(0x70);
    writeDataByte(0x70);
    
    // Unknown
    writeCommandByte(0x64);
    writeDataByte(0x28);
    writeDataByte(0x29);
    writeDataByte(0xF1);
    writeDataByte(0x01);
    writeDataByte(0xF1);
    writeDataByte(0x00);
    writeDataByte(0x07);
    
    // Unknown
    writeCommandByte(0x66);
    writeDataByte(0x3C);
    writeDataByte(0x00);
    writeDataByte(0xCD);
    writeDataByte(0x67);
    writeDataByte(0x45);
    writeDataByte(0x45);
    writeDataByte(0x10);
    writeDataByte(0x00);
    writeDataByte(0x00);
    writeDataByte(0x00);
    
    // Unknown
    writeCommandByte(0x67);
    writeDataByte(0x00);
    writeDataByte(0x3C);
    writeDataByte(0x00);
    writeDataByte(0x00);
    writeDataByte(0x00);
    writeDataByte(0x01);
    writeDataByte(0x54);
    writeDataByte(0x10);
    writeDataByte(0x32);
    writeDataByte(0x98);
    
    // Unknown
    writeCommandByte(0x74);
    writeDataByte(0x10);
    writeDataByte(0x85);
    writeDataByte(0x80);
    writeDataByte(0x00);
    writeDataByte(0x00);
    writeDataByte(0x4E);
    writeDataByte(0x00);
    
    // Unknown
    writeCommandByte(0x98);
    writeDataByte(0x3e);
    writeDataByte(0x07);

    writeCommandByte(GC9A01A_DISPLAY_INVERSION_ON);
	writeCommandByte(GC9A01A_SLEEP_OUT);
  	writeCommandByte(GC9A01A_PARTIAL_MODE_OFF);
  	writeCommandByte(GC9A01A_DISPLAY_ON);

    rotate(0);

    drawDisplay();
}

void displayGC9A01A::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void displayGC9A01A::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void displayGC9A01A::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void displayGC9A01A::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void displayGC9A01A::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayGC9A01A::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void displayGC9A01A::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void displayGC9A01A::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayGC9A01A::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void displayGC9A01A::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
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

void displayGC9A01A::fill(uint32_t colorR8G8B8)
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

void displayGC9A01A::drawDisplay()
{
	uint16_t xStart = 0 + mDisplayBuffer->getXShift();
    uint16_t xEnd = mDisplayBuffer->getWidth() + mDisplayBuffer->getXShift() - 1;
	uint16_t yStart = 0 + mDisplayBuffer->getYShift();
    uint16_t yEnd = mDisplayBuffer->getHeight() + mDisplayBuffer->getYShift() - 1;

	writeCommandByte(GC9A01A_COLUMN_ADDRESS_SET);
    uint8_t columnData[] = {(uint8_t)(xStart >> 8), (uint8_t)(xStart & 0xFF), (uint8_t)(xEnd >> 8), (uint8_t)(xEnd & 0xFF)};
    writeData(columnData, sizeof(columnData));

	writeCommandByte(GC9A01A_ROW_ADDRESS_SET);
    uint8_t rowData[] = {(uint8_t)(yStart >> 8), (uint8_t)(yStart & 0xFF), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xFF)};
    writeData(rowData, sizeof(rowData));
                    
	writeCommandByte(GC9A01A_MEMORY_WRITE);
    writeData(getDisplayBuffer()->getBuffer(), getDisplayBuffer()->getBufferSize());
}

void displayGC9A01A::brightness(uint8_t value)
{
    // Does not seem to work
    // writeCommand(GC9A01A_WRITE_DISPLAY_BRIGHTNESS);
    // writeDataByte(value);
}

void displayGC9A01A::contrast(uint8_t value)
{
    // NA 
}

void displayGC9A01A::invert(bool value)
{
    writeCommandByte(value ? GC9A01A_DISPLAY_INVERSION_OFF : GC9A01A_DISPLAY_INVERSION_ON);
}

void displayGC9A01A::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte(
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB);
    }
    else if (degrees == 90)
    {
        writeCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte((uint8_t)(
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 180)
    {
        writeCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
	    writeDataByte((uint8_t)(
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 270)
    {
        writeCommandByte(GC9A01A_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte((uint8_t)(
            
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            GC9A01A_MEMORY_ADDRESS_DATA_CONTROL_RGB));
    }
}