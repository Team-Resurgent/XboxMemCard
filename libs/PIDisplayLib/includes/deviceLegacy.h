#pragma once

#include "hardware/spi.h"
#include "hardware/structs/spi.h"

#define LEGACY_ROWS 4
#define LEGACY_COLS 20
#define LEGACY_BUFFER_SIZE 256

class deviceLegacy
{
public:
    deviceLegacy();
    void initSpi(spi_inst_t* spi, uint32_t baudRate);
    int16_t peekCommand(uint16_t index);
    void completeCommand();
    void reset();
    void poll();
    
    uint8_t getRows();
    uint8_t getCols();
    uint8_t getCursorRow();
    uint8_t getCursorCol();
    uint8_t getBrightness();
    uint8_t getContrast();
    uint8_t getDisplayChar(uint8_t row, uint8_t col);
    bool getShowDisplay();
    bool getShowCursor();

private:
    spi_inst_t* mSpi;
    int16_t mBuffer[LEGACY_BUFFER_SIZE];
    uint16_t mBufferRxPos;
    uint16_t mBufferTxPos;
    uint8_t mCursorPosRow;
    uint8_t mCursorPosCol;

    uint8_t mBrightness;
    uint8_t mContrast;
    bool mShowDisplay;
    bool mShowCursor;
    bool mWrapping;
    bool mScrolling;

    uint8_t mDisplayBuffer[LEGACY_ROWS * LEGACY_COLS];
};
