#include "sd_card_storage.h"

#include <stdio.h>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"

void sdtest() 
{
    time_init();

    puts("Hello, world!");

    sd_card_t *sdcard = sd_get_by_num(0);


    FRESULT result = f_mount(&sdcard->fatfs, sdcard->pcName, 1);

    if (FR_OK != result) 
    {
        panic("f_mount error: %s (%d)\n", FRESULT_str(result), result);
    }

    FIL fil;
    const char* const filename = "filename.txt";
    result = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != result && FR_EXIST != result)
    {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(result), result);
    }
    if (f_printf(&fil, "Hello, world!\n") < 0) 
    {
        printf("f_printf failed\n");
    }
    result = f_close(&fil);
    if (FR_OK != result) 
    {
        printf("f_close error: %s (%d)\n", FRESULT_str(result), result);
    }

    f_unmount(sdcard->pcName);

    printf("Goodbye, world!");
}



bool listFiles() 
{
    time_init();

    printf("Listing Files...\n");

    sd_card_t *sdcard = sd_get_by_num(0);

    FRESULT result = f_mount(&sdcard->fatfs, sdcard->pcName, 1);
    if (FR_OK != result) 
    {
        printf("Mount failed.\n");
        return false;
    }

    const char* path = "0:\\";

	DIR dirs;
	FRESULT fr = f_opendir(&dirs, path);
    if (fr == FR_OK)
    {
        FILINFO fileInfo;
		while (((fr = f_readdir(&dirs, &fileInfo)) == FR_OK) && fileInfo.fname[0]) 
        {
			if (fileInfo.fattrib & AM_DIR == 0) 
            {
				printf("%s/%s\n", path, fileInfo.fname);
			}
		}
	}

    f_unmount(sdcard->pcName);
    printf("Done.\n");
}