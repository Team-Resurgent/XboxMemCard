#include "sd_card_storage.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include <stdio.h>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"
#include "storage.h"

static FIL sd_card_mounted_file;

bool sd_card_write_sector(uint64_t sector, uint8_t *data, uint32_t data_size)
{
    FRESULT result;
    result = f_lseek(&sd_card_mounted_file, sector << XMU_SECTOR_SHIFT);
    if (result != FR_OK)
    {
        return false;
    }
    UINT bytes_written = 0;
    result = f_write(&sd_card_mounted_file, data, data_size, &bytes_written);
    return result == FR_OK && bytes_written == data_size;
}

bool sd_card_read_sector(uint64_t sector, uint8_t *data, uint32_t data_size)
{
    FRESULT result;
    result = f_lseek(&sd_card_mounted_file, sector << XMU_SECTOR_SHIFT);
    if (result != FR_OK)
    {
        return false;
    }
    UINT bytes_read = 0;
    result = f_read(&sd_card_mounted_file, data, data_size, &bytes_read);
    return result == FR_OK && bytes_read == data_size;
}

uint64_t sd_card_get_capcity()
{
    FSIZE_t size = f_size(&sd_card_mounted_file);
    return size;
}

bool sd_card_init()
{
    FRESULT result;

    sd_card_t* sd_card = sd_get_by_num(0);
 
    result = f_mount(&sd_card->fatfs, sd_card->pcName, 1);
    if (FR_OK != result) 
    {
        return false;
    }

    result = f_open(&sd_card_mounted_file, "0:\\XboxMu.iso", FA_OPEN_EXISTING | FA_READ | FA_WRITE);
    if (result != FR_OK)
    {
        f_unmount(sd_card->pcName);
        return false;
    }

    return true;
}

void sd_card_close()
{
    f_close(&sd_card_mounted_file);

    sd_card_t* sd_card = sd_get_by_num(0);
    f_unmount(sd_card->pcName);
}