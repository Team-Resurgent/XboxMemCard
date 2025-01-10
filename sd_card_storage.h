#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool sd_card_write_sector(uint64_t sector, uint8_t *data, uint32_t data_size);
bool sd_card_read_sector(uint64_t sector, uint8_t *data, uint32_t data_size);
uint64_t sd_card_get_capcity();
bool sd_card_init();
void sd_card_close();

#ifdef __cplusplus
}
#endif

//http://elm-chan.org/fsw/ff/00index_e.html