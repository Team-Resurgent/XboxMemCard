#ifndef USBD_TOP_H_
#define USBD_TOP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <tusb.h>
#include <device/usbd_pvt.h>

static const tusb_desc_device_t XID_DESC_DEVICE =
{
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0110,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = 0x045E,
    .idProduct = 0x0280,
    .bcdDevice = 0x0121,

    .iManufacturer = 0x00,
    .iProduct = 0x00,
    .iSerialNumber = 0x00,

    .bNumConfigurations = 0x01
};

enum
{
  ITF_NUM_MSC,
  ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN \
    (TUD_CONFIG_DESC_LEN) + \
    (TUD_MSC_DESC_LEN)

static uint8_t const XID_DESC_CONFIGURATION[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 500),


#if (CFG_TUD_MSC >= 1)
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, ITF_NUM_MSC + 1, 0x80 | (ITF_NUM_MSC + 1), 64),
#endif

};


uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&XID_DESC_DEVICE;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return XID_DESC_CONFIGURATION;
}

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)index;
    (void)langid;
    return NULL;
}


#ifdef __cplusplus
}
#endif

#endif //USBD_TOP_H_