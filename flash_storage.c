#include "flash_storage.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <hardware/sync.h>
#include <hardware/flash.h>
#include "storage.h"

#define XMU_SECTORS_PER_FLASH_SECTOR 8

#define FLASH_SECTOR_SHIFT 12
#define FLASH_TARGET_OFFSET (256 * 1024)

#define STORAGE_CMD_TOTAL_BYTES 64

const uint8_t *flash_target_contents = (uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);

bool flash_write_sector(uint64_t sector, uint8_t *data, uint32_t data_size)
{
  if (data_size > XMU_SECTOR_SIZE)
  {
    return false;
  }

  uint8_t *tempFlashSector = (uint8_t *)malloc(FLASH_SECTOR_SIZE);
  uint32_t sectorOffset = sector << XMU_SECTOR_SHIFT;
  uint32_t flashSector = sectorOffset >> FLASH_SECTOR_SHIFT;
  uint32_t flashSectorOffset = flashSector << FLASH_SECTOR_SHIFT;
  memcpy(tempFlashSector, flash_target_contents + flashSectorOffset, FLASH_SECTOR_SIZE);
  uint32_t tempFlashOffset = (sector & (XMU_SECTORS_PER_FLASH_SECTOR - 1)) << XMU_SECTOR_SHIFT;
  memcpy(tempFlashSector + tempFlashOffset, data, data_size);

  uint32_t ints = save_and_disable_interrupts();
  flash_range_erase(FLASH_TARGET_OFFSET + flashSectorOffset, FLASH_SECTOR_SIZE);
  flash_range_program(FLASH_TARGET_OFFSET + flashSectorOffset, tempFlashSector, FLASH_SECTOR_SIZE);
  restore_interrupts(ints);

  free(tempFlashSector);
  return true;
}

bool flash_read_sector(uint64_t sector, uint8_t *data, uint32_t data_size)
{
  if (data_size > XMU_SECTOR_SIZE)
  {
    return false;
  }

  uint32_t sectorOffset = sector << XMU_SECTOR_SHIFT;
  memcpy(data, flash_target_contents + sectorOffset, data_size);
  return true;
}

uint64_t flash_get_capcity()
{
    static uint64_t capacity = 0;
    if (capacity == 0)
    {
      uint8_t txbuf[STORAGE_CMD_TOTAL_BYTES] = {0x9f};
      uint8_t rxbuf[STORAGE_CMD_TOTAL_BYTES] = {0};
      flash_do_cmd(txbuf, rxbuf, STORAGE_CMD_TOTAL_BYTES);
      printf("Flash Manufacturer %02x, Type %02x, CapacityCode %02x\n", rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3]);
      capacity = (1ULL << rxbuf[3]) - (256 * 1024);
    }
    return capacity;
}

bool flash_init()
{
#if CONFIG_TYPE == 1 // xboxog

  if (flash_target_contents[0] != 0x46 || flash_target_contents[1] != 0x41 || flash_target_contents[2] != 0x54 || flash_target_contents[3] != 0x58)
  {
    printf("Init storage:\n");

    uint8_t sector[XMU_SECTOR_SIZE];

    memset(sector, 0x00, XMU_SECTOR_SIZE);
    for (uint32_t i = 8; i <= 15; i++)
    {
      flash_write_sector(i, sector, XMU_SECTOR_SIZE);
    }

    memset(sector, 0xff, XMU_SECTOR_SIZE);
    for (uint32_t i = 1; i <= 7; i++)
    {
      flash_write_sector(i, sector, XMU_SECTOR_SIZE);
    }
    for (uint32_t i = 16; i <= 47; i++)
    {
      flash_write_sector(i, sector, XMU_SECTOR_SIZE);
    }

    // header
    sector[0] = 0x46;
    sector[1] = 0x41;
    sector[2] = 0x54;
    sector[3] = 0x58;

    // serial
    sector[4] = 0x01;
    sector[5] = 0x02;
    sector[6] = 0x03;
    sector[7] = 0x04;

    // sectors per cluster
    sector[8] = 0x04;
    sector[9] = 0x00;
    sector[10] = 0x00;
    sector[11] = 0x00;

    // root dir first cluster
    sector[12] = 0x01;
    sector[13] = 0x00;
    sector[14] = 0x00;
    sector[15] = 0x00;

    // name
    sector[16] = 'X';
    sector[17] = 0;
    sector[18] = 'B';
    sector[19] = 0;
    sector[20] = 'O';
    sector[21] = 0;
    sector[22] = 'X';
    sector[23] = 0;
    sector[24] = ' ';
    sector[25] = 0;
    sector[26] = 'M';
    sector[27] = 0;
    sector[28] = 'E';
    sector[29] = 0;
    sector[30] = 'M';
    sector[31] = 0;
    sector[32] = ' ';
    sector[33] = 0;
    sector[34] = 'T';
    sector[35] = 0;
    sector[36] = 'R';
    sector[37] = 0;
    sector[38] = 0;
    sector[39] = 0;

    flash_write_sector(0, sector, XMU_SECTOR_SIZE);
  }

#elif CONFIG_TYPE == 2 // unformatted

  // dont need to do anything

#endif

  return true;
}

void flash_close()
{
}
