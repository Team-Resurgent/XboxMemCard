#include "deviceLegacy.h"

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/structs/spi.h"

#include <cstdio>
#include <cstring>

#define SPI_LEGACY_TX 15
#define SPI_LEGACY_SCK 14
#define SPI_LEGACY_CSN 13
#define SPI_LEGACY_RX 12

#define LEGACY_CURSOR_UP 65
#define LEGACY_CURSOR_DOWN 66
#define LEGACY_CURSOR_RIGHT 67
#define LEGACY_CURSOR_LEFT 68

#define LEGACY_CURSOR_HOME 1
#define LEGACY_HIDE_DISPLAY 2
#define LEGACY_SHOW_DISPLAY 3
#define LEGACY_HIDE_CURSOR 4
#define LEGACY_SHOW_UNDERLINE_CURSOR 5
#define LEGACY_SHOW_BLOCK_CURSOR 6
#define LEGACY_SHOW_INVERTED_CURSOR 7
#define LEGACY_BACKSPACE 8
#define LEGACY_MODULE_CONFIG 9
#define LEGACY_LINE_FEED 10
#define LEGACY_DELETE_IN_PLACE 11
#define LEGACY_FORM_FEED 12
#define LEGACY_CARRIAGE_RETURN 13
#define LEGACY_SET_BACKLIGHT 14
#define LEGACY_SET_CONTRAST 15
#define LEGACY_SET_CURSOR_POSITION 17
#define LEGACY_DRAW_BAR_GRAPH 18 
#define LEGACY_SCROLL_ON 19
#define LEGACY_SCROLL_OFF 20
#define LEGACY_WRAP_ON 23
#define LEGACY_WRAP_OFF 24
#define LEGACY_CUSTOM_CHARACTER 25
#define LEGACY_REBOOT 26
#define LEGACY_CURSOR_MOVE 27
#define LEGACY_LARGE_NUMBER 28

deviceLegacy::deviceLegacy()
{
    memset(mBuffer, -1, sizeof(mBuffer));
    mBufferRxPos = 0;
    mBufferTxPos = 0;

    mBrightness = 100;
    mContrast = 0;

    reset();
}

void deviceLegacy::initSpi(spi_inst_t* spi, uint32_t baudRate)
{
    mSpi = spi;

    spi_init(mSpi, baudRate);
    spi_set_slave(mSpi, true);
    gpio_set_function(SPI_LEGACY_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_LEGACY_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_LEGACY_TX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_LEGACY_CSN, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func((uint32_t)SPI_LEGACY_RX, (uint32_t)SPI_LEGACY_TX, (uint32_t)SPI_LEGACY_SCK, (uint32_t)SPI_LEGACY_CSN, GPIO_FUNC_SPI));

    spi_set_format(mSpi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
}

int16_t deviceLegacy::peekCommand(uint16_t index) 
{
    return mBuffer[(mBufferTxPos + index) % LEGACY_BUFFER_SIZE];
}

void deviceLegacy::completeCommand() 
{
    mBuffer[mBufferTxPos];
    mBufferTxPos = (mBufferTxPos + 1) % LEGACY_BUFFER_SIZE;
}

void deviceLegacy::reset()
{
    mCursorPosRow = 0;
    mCursorPosCol = 0;
    mShowDisplay = false;
    mShowCursor = true;
    mWrapping = false;
    mScrolling = false;
    for (int i = 0; i < LEGACY_ROWS * LEGACY_COLS; i++)
    {
        mDisplayBuffer[i] = ' ';
    }
    const char* message = "Please Wait...";
    memcpy(mDisplayBuffer, message, strlen(message));
}

void deviceLegacy::poll()
{
    while (spi_is_readable(mSpi)) 
    {
        uint8_t value = 0;
        spi_read_blocking(mSpi,  0, &value, 1);
        mBuffer[mBufferRxPos] = value;
        mBufferRxPos = (mBufferRxPos + 1) % LEGACY_BUFFER_SIZE;
    }

     if (mBufferRxPos != mBufferTxPos) 
     {
        int16_t peekedCommand = peekCommand(0);
        switch (peekedCommand) 
        {
            case -1:
                break;  
            case LEGACY_CURSOR_HOME:
                mCursorPosRow = 0;
                mCursorPosCol = 0;
                completeCommand();
                break;
            case LEGACY_HIDE_DISPLAY:
                mShowDisplay = false;
                completeCommand();
                break;
            case LEGACY_SHOW_DISPLAY:
                mShowDisplay = true;
                completeCommand();
                break;
            case LEGACY_HIDE_CURSOR:
                mShowCursor = false;
                completeCommand();
                break;
            case LEGACY_SHOW_UNDERLINE_CURSOR:
            case LEGACY_SHOW_BLOCK_CURSOR:
            case LEGACY_SHOW_INVERTED_CURSOR:
                mShowCursor = true;
                completeCommand();
                break;
            case LEGACY_BACKSPACE:
                if (mCursorPosCol > 0) 
                {
                    mCursorPosCol--;
                    mBuffer[(mCursorPosRow * LEGACY_COLS) + mCursorPosCol] = ' ';
                }
                completeCommand();
                break;
            case LEGACY_LINE_FEED:
                if (mCursorPosRow < LEGACY_ROWS - 1) 
                {
                    mCursorPosRow++;
                }
                completeCommand();
                break;
            case LEGACY_DELETE_IN_PLACE: 
                mBuffer[(mCursorPosRow * LEGACY_COLS) + mCursorPosCol] = ' ';
                completeCommand();
                break;
            case LEGACY_FORM_FEED: 
                for (int i = 0; i < LEGACY_ROWS * LEGACY_COLS; i++)
                {
                    mBuffer[i] = ' ';
                }
                mCursorPosRow = 0;
                mCursorPosCol = 0;
                completeCommand();
                break;
            case LEGACY_CARRIAGE_RETURN: 
                mCursorPosCol = 0;
                completeCommand();
                break;
            case LEGACY_SET_CURSOR_POSITION: 
                if (peekCommand(2) != -1) 
                {
                    uint8_t col = mBuffer[(uint8_t)(mBufferTxPos + 1)];
                    uint8_t row = mBuffer[(uint8_t)(mBufferTxPos + 2)];
                    if (col < LEGACY_COLS && row < LEGACY_ROWS) {
                        mCursorPosCol = col;
                        mCursorPosRow = row;
                    }
                    completeCommand();
                    completeCommand();
                    completeCommand();
                }
                break;
            case LEGACY_SET_BACKLIGHT:
                if (peekCommand(1) != -1) 
                { 
                    uint8_t brightness = mBuffer[(uint8_t)(mBufferTxPos + 1)];
                    if (brightness >= 0 && brightness <= 100) 
                    {
                        mBrightness = brightness;
                    }
                    completeCommand();
                    completeCommand();
                }
                break;
            case LEGACY_SET_CONTRAST:
                if (peekCommand(1) != -1) 
                { 
                    uint8_t contrast = mBuffer[(uint8_t)(mBufferTxPos + 1)];
                    if (contrast >= 0 && contrast <= 100) 
                    {
                        mContrast = contrast;
                    }
                    completeCommand();
                    completeCommand();
                }
                break;
            case LEGACY_REBOOT:
                reset();
                completeCommand();
                break;
            case LEGACY_CURSOR_MOVE:
                if (peekCommand(1) == 27 && peekCommand(2) != -1) 
                {
                    switch (peekCommand(2)) 
                    {
                        case LEGACY_CURSOR_UP:
                            if (mCursorPosRow > 0) 
                            {
                                mCursorPosRow--;
                            }
                            break;
                        case LEGACY_CURSOR_DOWN:
                            if (mCursorPosRow < (LEGACY_ROWS - 1) ) 
                            {
                                mCursorPosRow++;
                            }
                            break;
                        case LEGACY_CURSOR_RIGHT:
                            if (mCursorPosCol < (LEGACY_COLS - 1)) 
                            {
                                mCursorPosCol++;
                            }
                            break;
                        case LEGACY_CURSOR_LEFT:
                            if (mCursorPosCol > 0) 
                            {
                                mCursorPosCol--;
                            }
                            break;
                        default:
                            break;
                    }
                    completeCommand();
                    completeCommand();
                    completeCommand();
                }
                break;
            case LEGACY_WRAP_OFF:
                mWrapping = false;
                completeCommand();
                break;
            case LEGACY_WRAP_ON:
                mWrapping = true;
                completeCommand();
                break;
            case LEGACY_SCROLL_OFF:
                mScrolling = false;
                completeCommand();
                break;
            case LEGACY_SCROLL_ON:
                mScrolling = true;
                completeCommand();
                break;
            case 32 ... 255:
                mDisplayBuffer[(mCursorPosRow * LEGACY_COLS) + mCursorPosCol] = peekedCommand;
                if (mCursorPosCol < LEGACY_COLS) 
                {
                    mCursorPosCol++;
                }
                completeCommand();
                break;
            case LEGACY_LARGE_NUMBER:
            case LEGACY_DRAW_BAR_GRAPH:
            case LEGACY_MODULE_CONFIG:
            case LEGACY_CUSTOM_CHARACTER:
            default:
                completeCommand();
                break;
        }
     }
}

uint8_t deviceLegacy::getRows()
{
    return LEGACY_ROWS;
}

uint8_t deviceLegacy::getCols()
{
    return LEGACY_COLS;
}

uint8_t deviceLegacy::getCursorRow()
{
    return mCursorPosRow;
}

uint8_t deviceLegacy::getCursorCol()
{
    return mCursorPosCol;
}

uint8_t deviceLegacy::getBrightness()
{
    return mBrightness;
}

uint8_t deviceLegacy::getContrast()
{
    return mContrast;
}

uint8_t deviceLegacy::getDisplayChar(uint8_t row, uint8_t col)
{
    return mDisplayBuffer[(row * LEGACY_COLS) + col];
}

bool deviceLegacy::getShowDisplay()
{
    return mShowDisplay;
}

bool deviceLegacy::getShowCursor()
{
    return mShowCursor;
}