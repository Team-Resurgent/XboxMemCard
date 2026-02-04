# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
set(PROJECT_NAME "XboxMemCard" CACHE STRING "Project name")
set(CONFIG_TYPE 1 CACHE STRING "Config type") #1 = xboxog #2 = unformatted

message(STATUS "PICO_BOARD = ${PICO_BOARD}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")
message(STATUS "CONFIG_TYPE = ${CONFIG_TYPE}")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(${PROJECT_NAME} C CXX ASM)

add_compile_definitions(
  PICO_FLASH_SIZE_BYTES=16777216
  CONFIG_TYPE=${CONFIG_TYPE}
)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${PROJECT_NAME})

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

add_subdirectory(libs/FatFsSPI)
add_subdirectory(libs/PIDisplayLib)

target_sources(
  ${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/msc_disk.c
  ${CMAKE_CURRENT_LIST_DIR}/sd_card_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/flash_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(
  ${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)


# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(${PROJECT_NAME} PUBLIC
  pico_stdlib
  pico_unique_id
  pico_multicore
  tinyusb_device
  tinyusb_board
  hardware_spi
  hardware_i2c
  FatFsSPI
  PIDisplayLib
)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(${PROJECT_NAME} PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

pico_add_extra_outputs(${PROJECT_NAME})

# add url via pico_set_program_url
