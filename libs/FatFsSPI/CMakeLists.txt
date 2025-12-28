add_library(FatFsSPI INTERFACE)

target_sources(FatFsSPI INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/ff15/source/ffsystem.c
    ${CMAKE_CURRENT_LIST_DIR}/ff15/source/ffunicode.c
    ${CMAKE_CURRENT_LIST_DIR}/ff15/source/ff.c
    ${CMAKE_CURRENT_LIST_DIR}/sd_driver/sd_spi.c
    ${CMAKE_CURRENT_LIST_DIR}/sd_driver/hw_config.c
    ${CMAKE_CURRENT_LIST_DIR}/sd_driver/spi.c
    ${CMAKE_CURRENT_LIST_DIR}/sd_driver/sd_card.c
    ${CMAKE_CURRENT_LIST_DIR}/sd_driver/crc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/glue.c
    ${CMAKE_CURRENT_LIST_DIR}/src/f_util.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ff_stdio.c
    ${CMAKE_CURRENT_LIST_DIR}/src/debug.c
)

target_include_directories(FatFsSPI INTERFACE
    ff15/source
    sd_driver
    includes
)

target_link_libraries(FatFsSPI INTERFACE
        hardware_spi
        hardware_dma
        hardware_rtc
        pico_stdlib
)