add_library(middleware INTERFACE)
target_sources(
    middleware INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/i2c_mgr/i2c_mgr.c
                         ${CMAKE_CURRENT_SOURCE_DIR}/adc_mgr/adc_mgr.c
)
