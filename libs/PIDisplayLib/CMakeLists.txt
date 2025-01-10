add_library(PIDisplayLib INTERFACE)

target_sources(PIDisplayLib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/fonts.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/deviceLegacy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/deviceTouch.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displayBuffer.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displayDriver.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displayGC9A01A.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displayILI9341.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displaySH1106.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displaySH1122.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/src/displayST7789.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/displaySSD1306.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/displaySSD1309.cpp
)

target_include_directories(PIDisplayLib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/includes
)

target_link_libraries(PIDisplayLib INTERFACE
    pico_stdlib
    hardware_spi
    hardware_i2c
)