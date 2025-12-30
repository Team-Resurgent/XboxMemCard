# XboxMemCard

**XboxMemCard** transforms your **RP2040/RP2350** into a flash storage device compatible with your **Xbox Original** or **PC**.

You can choose from three build options:

---

## 1. Xbox OG (Auto-Formatted)

This build **automatically formats the storage to FATX**, making it ready to use immediately on your Xbox Original.  
It utilizes the Pi's **internal flash**, reserving approximately **256 KB** for system use.

---

## 2. Unformatted (Custom Format)

This build provides **raw storage**, allowing you to format it to any filesystem of your choice for use on an Xbox or PC.  
It also uses the Pi's **internal flash**, with about **256 KB** reserved for system storage.

---

## 3. SD Card Storage

This build allows you to **preload an SD card** with a file named image.bin

This file represents a flash storage image for use on your Xbox or PC.

**SD card GPIO pin connections:**

| Function      | GPIO Pin |
|---------------|----------|
| SPI_SCK_PIN   | 2        |
| SPI_MOSI_PIN  | 3        |
| SPI_MISO_PIN  | 4        |
| SPI_SS_PIN    | 5        |
