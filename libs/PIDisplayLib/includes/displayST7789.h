#pragma once

#include "displayDriver.h"
#include "displayBuffer.h"
#include "fonts.h"
#include "hardware/spi.h"
#include "hardware/structs/spi.h"

// Config
#define DISPLAY_ST7789_SPI spi0
#define DISPLAY_ST7789_BAUDRATE (10 * 1024 * 1024)
#define DISPLAY_ST7789_WIDTH 240
#define DISPLAY_ST7789_HEIGHT 320
#define DISPLAY_ST7789_X_SHIFT 0 
#define DISPLAY_ST7789_Y_SHIFT 0
#define DISPLAY_ST7789_BITS_PER_PIXEL 16

class displayST7789 : displayDriver
{
public:
    displayST7789();

    void drawChar(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, char character);
    void drawString(uint32_t colorR8G8B8, FontDef font, uint16_t x, uint16_t y, const char *message);

    void drawLine(uint32_t colorR8G8B8, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void drawRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void drawTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void drawCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius);
	void drawFilledRectangle(uint32_t colorR8G8B8, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	void drawFilledTriangle(uint32_t colorR8G8B8, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void drawFilledCircle(uint32_t colorR8G8B8, int16_t x, int16_t y, int16_t radius);

    void drawPixel(uint32_t colorR8G8B8, uint16_t x, uint16_t y);
    void fill(uint32_t colorR8G8B8);
    void drawDisplay();

    void brightness(uint8_t value);
    void contrast(uint8_t value);
    void invert(bool value);
    void rotate(uint16_t degrees);
};
