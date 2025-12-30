// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "bsp/board.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "tusb.h"

#include "flash_storage.h"
#include "sd_card_storage.h"
#include "fonts.h"
#include "pixelDisplaySSD1306.h"

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

void core1_entry() 
{
  //printf("Hello core 1\n");

  // displayDriver* display = (displayDriver*)new displaySSD1306();
  // display->rotate(180);
  // while (true)
  // {
  //   display->fill(0x000000);
  //   display->drawString(0xffffff, fonts::Font_12x16(), 8, 0, "Hello");
  //   display->drawString(0xffffff, fonts::Font_12x16(), 8, 16, "World");
  //   display->drawDisplay();
  //   sleep_ms(100);
  // }
}

int main(void)
{
  stdio_init_all();

  //multicore_launch_core1(core1_entry);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

#if CONFIG_TYPE == 1 || CONFIG_TYPE == 2
  flash_init();
#elif CONFIG_TYPE == 3
  sd_card_init();
#endif 

  tusb_init();

  while (true)
  {
    tud_task();
    led_blinking_task();
  }

  return 0;
}