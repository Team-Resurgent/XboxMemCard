#include "displayST7789.h"
#include "color.h"
#include "fonts.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define ST7789_NOP 0x00
#define ST7789_SOFTWARE_RESET 0x01
#define ST7789_SLEEP_IN 0x10
#define ST7789_SLEEP_OUT 0x11
#define ST7789_PARTIAL_MODE_ON 0x12
#define ST7789_PARTIAL_MODE_OFF 0x13
#define ST7789_DISPLAY_INVERSION_OFF 0x20
#define ST7789_DISPLAY_INVERSION_ON 0x21
#define ST7789_GAMMA_SET 0x26 // 2 Byte Command
#define ST7789_DISPLAY_OFF 0x28
#define ST7789_DISPLAY_ON 0x29
#define ST7789_COLUMN_ADDRESS_SET 0x2A // 5 Byte Command
#define ST7789_ROW_ADDRESS_SET 0x2B // 5 Byte Command
#define ST7789_MEMORY_WRITE 0x2C // 2+ Byte Command
#define ST7789_PARTIAL_START_END 0x30 // 5 Byte Command
#define ST7789_VERTICAL_SCROLLING_DIRECTION 0x33 // 7 Byte Command
#define ST7789_TEARING_EFFECT_LINE_OFF 0x34
#define ST7789_TEARING_EFFECT_LINE_ON 0x35
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL 0x36 // 2 Byte Command
#define ST7789_VERTICAL_SCROLLING_START_ADDRESS 0x37 // 3 Byte Command
#define ST7789_IDLE_MODE_OFF 0x38
#define ST7789_IDLE_MODE_ON 0x39
#define ST7789_INTERFACE_PIXEL_FORMAT 0x3A // 2 Byte Command
#define ST7789_MEMORY_WRITE_CONTINUE 0x3C // 2+ Byte Command
#define ST7789_SET_TEAR_SCANLINE 0x44 // 3 Byte Command
#define ST7789_WRITE_DISPLAY_BRIGHTNESS 0x51 // 2 Byte Command
#define ST7789_WRITE_CTRL_DISPLAY 0x53 // 2 Byte Command
#define ST7789_CONTENT_ADAPTIVE_BRIGHTNESS 0x55 // 2 Byte Command
#define ST7789_WRITE_CABC_MINIMUM_BRIGHTNESS 0x5E // 2 Byte Command

#define ST7789_RAM_CONTROL 0xB0 // 3 Byte Command
#define ST7789_RGB_CONTROL 0xB1 // 4 Byte Command
#define ST7789_PORCH_CONTROL 0xB2 // 6 Byte Command
#define ST7789_FRAME_RATE_CONTROL1 0xB3 // 4 Byte Command
#define ST7789_PARTIAL_CONTROL 0xB5 // 2 Byte Command
#define ST7789_GATE_CONTROL1 0xB7 // 2 Byte Command
#define ST7789_GATE_ON_TIMING_ADJUSTMENT 0xB8 // 5 Byte Command
#define ST7789_DIGITAL_GAMMA_ENABLE 0xBA // 2 Byte Command
#define ST7789_VCOM_SETTING 0xBB // 2 Byte Command
#define ST7789_POWER_SAVING_MODE 0xBC // 2 Byte Command
#define ST7789_DISPLAY_OFF_POWER_SAVE 0xBD // 2 Byte Command
#define ST7789_LCM_CONTROL 0xC0 // 2 Byte Command
#define ST7789_ID_SETTING 0xC1 // 4 Byte Command
#define ST7789_VDV_VRH_COMMAND_ENABLE 0xC2 // 3 Byte Command
#define ST7789_VRH_SET 0xC3 // 2 Byte Command
#define ST7789_VDH_SET 0xC4 // 2 Byte Command
#define ST7789_VCOM_OFFSET_SET 0xC5 // 2 Byte Command
#define ST7789_FRAME_RATE_CONTROL2 0xC6 // 2 Byte Command
#define ST7789_CABC_CONTROL 0xC7 // 2 Byte Command
#define ST7789_REGISTER_VALUE_SELECTION1 0xC8 // 2 Byte Command
#define ST7789_REGISTER_VALUE_SELECTION2 0xCA // 2 Byte Command
#define ST7789_POWER_CONTROL1 0xD0 // 3 Byte Command
#define ST7789_ENABLE_VAP_VAN_SIGNAL_OUTPUT 0xD2 // 2 Byte Command
#define ST7789_COMMAND2_ENABLE 0xDF // 2 Byte Command
#define ST7789_POSITIVE_VOLTAGE_GAMMA_CONTROL 0xE0 // 15 Byte Command
#define ST7789_NEGATIVE_VOLTAGE_GAMMA_CONTROL 0xE1 // 15 Byte Command
#define ST7789_DIGITAL_GAMMA_LOOKUP_TABLE_RED 0xE2 // 65 Byte Command
#define ST7789_DIGITAL_GAMMA_LOOKUP_TABLE_BLUE 0xE3 // 65 Byte Command
#define ST7789_GATE_CONTROL2 0xE4 // 4 Byte Command
#define ST7789_SPI2_ENABLE 0xE7 // 2 Byte Command
#define ST7789_POWER_CONTROL2 0xE8 // 2 Byte Command
#define ST7789_EQUALIZE_TIME_CONTROL 0xE9 // 4 Byte Command
#define ST7789_PROGRAM_CONTROL 0xEC // 2 Byte Command
#define ST7789_PROGRAM_MODE_ENABLE 0xFA // 5 Byte Command
#define ST7789_NVM_SETTING 0xFC // 3 Byte Command
#define ST7789_PROGRAM_ACTION 0xFE // 3 Byte Command

#define ST7789_INTERFACE_PIXEL_FORMAT_16BIT 0x05
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL_MY 0x80
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL_MX 0x40
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL_MV 0x20
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL_ML 0x10
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL_BGR 0x08
#define ST7789_MEMORY_ADDRESS_DATA_CONTROL_RGB 0x00

displayST7789::displayST7789()
{
    initDisplayBuffer(
        DISPLAY_ST7789_WIDTH, 
        DISPLAY_ST7789_HEIGHT, 
        DISPLAY_ST7789_X_SHIFT,
        DISPLAY_ST7789_Y_SHIFT,
        DISPLAY_ST7789_BITS_PER_PIXEL
    );

    initSpi(DISPLAY_ST7789_SPI, DISPLAY_ST7789_BAUDRATE);

    writeCommandByte(ST7789_SOFTWARE_RESET);
    sleep_ms(100);

    writeCommandByte(ST7789_INTERFACE_PIXEL_FORMAT);	
    writeDataByte(ST7789_INTERFACE_PIXEL_FORMAT_16BIT);

  	writeCommandByte(ST7789_PORCH_CONTROL);	
	uint8_t porchData[] = { 0x0c, 0x0c, 0x00, 0x33, 0x33 };
	writeData(porchData, sizeof(porchData));

    writeCommandByte(ST7789_GATE_CONTROL1);
    writeDataByte(0x35);
    writeCommandByte(ST7789_VCOM_SETTING);
    writeDataByte(0x20);
    writeCommandByte(ST7789_LCM_CONTROL);
    writeDataByte(0x2c);
    writeCommandByte(ST7789_VDV_VRH_COMMAND_ENABLE);	
    writeDataByte(0x01);
    writeCommandByte(ST7789_VRH_SET);	
    writeDataByte(0x0b);		
    writeCommandByte(ST7789_VDH_SET);	
    writeDataByte(0x20);		
    writeCommandByte(ST7789_FRAME_RATE_CONTROL2);		
    writeDataByte(0x0F);		
    writeCommandByte(ST7789_POWER_CONTROL1);	

	uint8_t powerData[] = { 0xa4, 0xa1 };
	writeData(powerData, sizeof(powerData));

	writeCommandByte(ST7789_POSITIVE_VOLTAGE_GAMMA_CONTROL);
    uint8_t positiveGammaData[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
    writeData(positiveGammaData, sizeof(positiveGammaData));

    writeCommandByte(ST7789_NEGATIVE_VOLTAGE_GAMMA_CONTROL);
    uint8_t negativeGammaData[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
    writeData(negativeGammaData, sizeof(negativeGammaData));

    writeCommandByte(ST7789_DISPLAY_INVERSION_ON);
	writeCommandByte(ST7789_SLEEP_OUT);
  	writeCommandByte(ST7789_PARTIAL_MODE_OFF);
  	writeCommandByte(ST7789_DISPLAY_ON);

    rotate(0);

    drawDisplay();
}

void displayST7789::drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character)
{
    displayDriver::drawChar(colorR8G8B8, font, x, y, character);
}

void displayST7789::drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message)
{
    displayDriver::drawString(colorR8G8B8, font, x, y, message);
}

void displayST7789::drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    displayDriver::drawLine(colorR8G8B8, x0, y0, x1, y1);
}

void displayST7789::drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawRectangle(colorR8G8B8, x, y, width, height);
}

void displayST7789::drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayST7789::drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawCircle(colorR8G8B8, x, y, radius);
}

void displayST7789::drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    displayDriver::drawFilledRectangle(colorR8G8B8, x, y, width, height);
}

void displayST7789::drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    displayDriver::drawFilledTriangle(colorR8G8B8, x1, y1, x2, y2, x3, y3);
}

void displayST7789::drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius)
{
    displayDriver::drawFilledCircle(colorR8G8B8, x, y, radius);
}

void displayST7789::drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y)
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

void displayST7789::fill(uint32_t colorR8G8B8)
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

void displayST7789::drawDisplay()
{
	uint16_t xStart = 0 + mDisplayBuffer->getXShift();
    uint16_t xEnd = mDisplayBuffer->getWidth() + mDisplayBuffer->getXShift() - 1;
	uint16_t yStart = 0 + mDisplayBuffer->getYShift();
    uint16_t yEnd = mDisplayBuffer->getHeight() + mDisplayBuffer->getYShift() - 1;

	writeCommandByte(ST7789_COLUMN_ADDRESS_SET);
    uint8_t columnData[] = {(uint8_t)(xStart >> 8), (uint8_t)(xStart & 0xFF), (uint8_t)(xEnd >> 8), (uint8_t)(xEnd & 0xFF)};
    writeData(columnData, sizeof(columnData));

	writeCommandByte(ST7789_ROW_ADDRESS_SET);
    uint8_t rowData[] = {(uint8_t)(yStart >> 8), (uint8_t)(yStart & 0xFF), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xFF)};
    writeData(rowData, sizeof(rowData));

	writeCommandByte(ST7789_MEMORY_WRITE);
    writeData(getDisplayBuffer()->getBuffer(), getDisplayBuffer()->getBufferSize());
}

void displayST7789::brightness(uint8_t value)
{
    // Does not seem to work
    // writeCommand(ST7789_WRITE_DISPLAY_BRIGHTNESS);
    // writeDataByte(value);
}

void displayST7789::contrast(uint8_t value)
{
    // NA 
}

void displayST7789::invert(bool value)
{
    writeCommandByte(value ? ST7789_DISPLAY_INVERSION_OFF : ST7789_DISPLAY_INVERSION_ON);
}

void displayST7789::rotate(uint16_t degrees)
{
    mDisplayBuffer->setRotation(degrees);

    if (degrees == 0)
    {
        writeCommandByte(ST7789_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte(ST7789_MEMORY_ADDRESS_DATA_CONTROL_RGB);
    }
    else if (degrees == 90)
    {
        writeCommandByte(ST7789_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte((uint8_t)(
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 180)
    {
        writeCommandByte(ST7789_MEMORY_ADDRESS_DATA_CONTROL);	
	    writeDataByte((uint8_t)(
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_MX | 
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_RGB)
        );
    }
    else if (degrees == 270)
    {
        writeCommandByte(ST7789_MEMORY_ADDRESS_DATA_CONTROL);	
        writeDataByte((uint8_t)(
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_MV | 
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_MY | 
            ST7789_MEMORY_ADDRESS_DATA_CONTROL_RGB));
    }
}