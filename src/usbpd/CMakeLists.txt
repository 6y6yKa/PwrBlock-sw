add_library(usbpd INTERFACE)

target_sources(
    usbpd
    INTERFACE dpm/dpm.c
              dpm/dpm_usbpd_if.c
              bsp/bsp_pwr.c
              bsp/bsp_vbus.c
              app/usbpd_pwr_if.c
              app/usbpd_dpm_core.c
              usbpd_task.c
)

target_include_directories(
    usbpd INTERFACE app target ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(usbpd INTERFACE stm32_usbpd)
