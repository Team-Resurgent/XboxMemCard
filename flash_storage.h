#pragma once

#include <stdint.h>
#include <stdbool.h>

enum
{
  DISK_BLOCK_NUM2  = 16, // 8KB is the smallest size that Windows allows to mount
  DISK_BLOCK_SIZE2 = 512
};

#define INDEX_CONTENTS2 \
"<head><meta http-equiv=\"Refresh\" content=\"0;url=http://www.xbox-scene.info\"></head><body></body>"


#define XMU_SECTOR_SHIFT 9
#define XMU_SECTOR_SIZE (1u << XMU_SECTOR_SHIFT)

#define XMU_SECTORS_PER_FLASH_SECTOR 8
#define FLASH_SECTOR_SHIFT 12
#define FLASH_TARGET_OFFSET (256 * 1024)

#ifdef __cplusplus
extern "C" {
#endif

bool flash_write_sector(uint64_t sector, uint8_t *data, uint32_t data_size);
bool flash_read_sector(uint64_t sector, uint8_t *data, uint32_t data_size);
uint64_t flash_get_capcity();
bool flash_init();
void flash_close();

#ifdef __cplusplus
}
#endif