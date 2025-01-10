#include "deviceTouch.h"

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/structs/spi.h"

#include <cstdio>
#include <cstring>

#define SPI_TOUCH_SPI spi1
#define SPI_TOUCH_IRQ 11
#define SPI_TOUCH_RX 12
#define SPI_TOUCH_CSN 13
#define SPI_TOUCH_SCK 14
#define SPI_TOUCH_TX 15

#define MAX_LEN 3
#define	XPT_START 0x80
#define XPT_XPOS 0x50
#define XPT_YPOS 0x10
#define XPT_SER 0x04

void deviceTouch::initSpi(spi_inst_t* spi, uint32_t baudRate)
{
    mSpi = spi;

    gpio_init(SPI_TOUCH_IRQ);
    gpio_set_dir(SPI_TOUCH_IRQ, GPIO_IN);
    gpio_pull_up(SPI_TOUCH_IRQ);

    gpio_init(SPI_TOUCH_CSN);
    gpio_put(SPI_TOUCH_CSN, 1);
    gpio_set_dir(SPI_TOUCH_CSN, GPIO_OUT);

    spi_init(mSpi, baudRate);
    spi_set_slave(mSpi, false);
    gpio_set_function(SPI_TOUCH_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TOUCH_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TOUCH_TX, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func((uint32_t)SPI_TOUCH_RX, (uint32_t)SPI_TOUCH_TX, (uint32_t)SPI_TOUCH_SCK, (uint32_t)SPI_TOUCH_CSN, GPIO_FUNC_SPI));

    spi_set_format(mSpi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
}

//https://github.com/usemodj/esp8266_240x320_lcd_touch_buttons?tab=readme-ov-file
//https://github.com/spapadim/XPT2046/blob/master/XPT2046.cpp
//https://github.com/nopnop2002/Raspberry-ili9340spi/blob/master/xpt2046.c

uint16_t deviceTouch::readADC(uint8_t cmd)
{
    uint8_t temp;
    uint16_t value = 0;

    gpio_put(SPI_TOUCH_CSN, 0);
    spi_write_blocking(mSpi, &cmd, 1);
    sleep_us(200);

    spi_read_blocking(mSpi, 0, &temp, 1);
    value = temp << 8;
    spi_read_blocking(mSpi, 0, &temp, 1);
    value != temp;
    value >>= 3;   

    gpio_put(SPI_TOUCH_CSN, 1);
	return value;
}


#define READ_TIMES  5	//Number of readings
#define LOST_NUM    1	//Discard value

uint16_t deviceTouch::readAverageADC(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t Read_Buff[READ_TIMES];
    uint16_t Read_Sum = 0, Read_Temp = 0;

    //Read and save multiple samples
    for(uint8_t i = 0; i < READ_TIMES; i++){
		Read_Buff[i] = readADC(cmd);
		sleep_us(200);
	}

    //Sort from small to large
    for (i = 0; i < READ_TIMES  -  1; i ++) {
        for (j = i + 1; j < READ_TIMES; j ++) {
            if (Read_Buff[i] > Read_Buff[j]) {
                Read_Temp = Read_Buff[i];
                Read_Buff[i] = Read_Buff[j];
                Read_Buff[j] = Read_Temp;
            }
        }
    }

    //Exclude the largest and the smallest
    for (i = LOST_NUM; i < READ_TIMES - LOST_NUM; i ++)
        Read_Sum += Read_Buff[i];

    //Averaging
    Read_Temp = Read_Sum / (READ_TIMES - 2 * LOST_NUM);

    return Read_Temp;
}

void deviceTouch::readPositionADC(uint16_t& pXCh_Adc, uint16_t& pYCh_Adc)
{
    pXCh_Adc = readAverageADC(XPT_START | XPT_XPOS);
    pYCh_Adc = readAverageADC(XPT_START | XPT_YPOS);
}

#define ERR_RANGE 50	//tolerance scope

bool deviceTouch::readPositionTwiceADC(uint16_t& pXCh_Adc, uint16_t& pYCh_Adc)
{
    uint16_t XCh_Adc1, YCh_Adc1, XCh_Adc2, YCh_Adc2;

    //Read the ADC values Read the ADC values twice
    readPositionADC(XCh_Adc1, YCh_Adc1);
	sleep_us(10);
    readPositionADC(XCh_Adc2, YCh_Adc2);
	sleep_us(10);
	
    //The ADC error used twice is greater than ERR_RANGE to take the average
    if( ((XCh_Adc2 <= XCh_Adc1 && XCh_Adc1 < XCh_Adc2 + ERR_RANGE) ||
         (XCh_Adc1 <= XCh_Adc2 && XCh_Adc2 < XCh_Adc1 + ERR_RANGE))
        && ((YCh_Adc2 <= YCh_Adc1 && YCh_Adc1 < YCh_Adc2 + ERR_RANGE) ||
            (YCh_Adc1 <= YCh_Adc2 && YCh_Adc2 < YCh_Adc1 + ERR_RANGE))) {
        pXCh_Adc = (XCh_Adc1 + XCh_Adc2) / 2;
        pYCh_Adc = (YCh_Adc1 + YCh_Adc2) / 2;
        return true;
    }

    //The ADC error used twice is less than ERR_RANGE returns failed
    return false;
}

void deviceTouch::readTouchPos(uint16_t& x, uint16_t& y)
{
    bool noTouch = gpio_get(SPI_TOUCH_IRQ);
    if (noTouch == false)
    {
        bool error = readPositionTwiceADC(x, y);
        printf("x = %i, y = %i error = %i\n", x, y, error);
    }
}