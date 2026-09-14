add_library(stm32_usbpd INTERFACE)

target_compile_definitions(
    stm32_usbpd
    INTERFACE USBPD_PORT_COUNT=1
              USBPDCORE_LIB_PD3_CONFIG_MINSNK
              _SNK
              _RTOS
              osCMSIS=0x20000
)

target_include_directories(
    stm32_usbpd SYSTEM
    INTERFACE ${CMAKE_SOURCE_DIR}/lib/stm32_usbpd/core/inc
              ${CMAKE_SOURCE_DIR}/lib/stm32_usbpd/devices/stm32g0xx/inc
)

target_sources(
    stm32_usbpd
    INTERFACE core/src/usbpd_trace.c
              devices/stm32g0xx/src/usbpd_cad_hw_if.c
              devices/stm32g0xx/src/usbpd_hw.c
              devices/stm32g0xx/src/usbpd_hw_if_it.c
              devices/stm32g0xx/src/usbpd_phy.c
              devices/stm32g0xx/src/usbpd_phy_hw_if.c
              devices/stm32g0xx/src/usbpd_timersserver.c
)

target_link_directories(
    stm32_usbpd INTERFACE ${CMAKE_SOURCE_DIR}/lib/stm32_usbpd/core/lib
)

target_link_libraries(
    stm32_usbpd INTERFACE :USBPDCORE_PD3_CONFIG_MINSNK_CM0PLUS_wc32.a
                          cmsis_freertos
)

target_compile_options(stm32_usbpd INTERFACE -Wno-unused-parameter)
