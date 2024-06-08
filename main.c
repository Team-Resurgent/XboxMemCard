// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "flash_storage.h"
#include "sd_card_storage.h"

#include <string.h>
#include <stdlib.h>

static uint32_t blink_interval_ms = 1000;

void led_blinking_task(void)
{
  static uint32_t start_ms = 0;

  if (!blink_interval_ms || board_millis() - start_ms < blink_interval_ms)
  {
    return;
  }

  start_ms += blink_interval_ms;
  gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get_out_level(PICO_DEFAULT_LED_PIN));
}


// flash_get_unique_id

int main(void)
{
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  stdio_init_all();

  //flash_init();

  tusb_init();

  sleep_ms(2000);

  while (true)
  {
    listFiles();
    sleep_ms(5000);
  }

  while (true)
  {
    tud_task();
    led_blinking_task();
  }

  return 0;
}

// https://github.com/Ryzee119/ogx360_t4/blob/be37b41775778137f6e1918245771dc31b6afea9/src/xmu.cpp