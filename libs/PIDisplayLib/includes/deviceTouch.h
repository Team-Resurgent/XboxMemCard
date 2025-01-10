#pragma once

#include "hardware/spi.h"
#include "hardware/structs/spi.h"

class deviceTouch
{
public:
    void initSpi(spi_inst_t* spi, uint32_t baudRate);
    uint16_t readADC(uint8_t cmd);
    uint16_t readAverageADC(uint8_t cmd);
    void readPositionADC(uint16_t& pXCh_Adc, uint16_t& pYCh_Adc);
    bool readPositionTwiceADC(uint16_t& pXCh_Adc, uint16_t& pYCh_Adc);
    void readTouchPos(uint16_t& x, uint16_t& y);
private:
    spi_inst_t* mSpi;
};