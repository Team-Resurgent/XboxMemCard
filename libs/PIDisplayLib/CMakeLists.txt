add_library(PIDisplayLib INTERFACE)

target_sources(PIDisplayLib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/fonts.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/deviceLegacy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/deviceEEPROM.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/deviceTouch.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displayBase.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplayBuffer.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplayDriver.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplayGC9A01A.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplayILI9341.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplaySH1106.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplaySH1122.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplayST7789.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplaySSD1306.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/pixelDisplaySSD1309.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/textDisplayDriver.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/textDisplayUS2066.cpp 
)

target_include_directories(PIDisplayLib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/includes
)

target_link_libraries(PIDisplayLib INTERFACE
    pico_stdlib
    hardware_spi
    hardware_i2c
)