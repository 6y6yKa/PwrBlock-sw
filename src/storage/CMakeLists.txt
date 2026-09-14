add_library(storage INTERFACE)
target_sources(storage INTERFACE storage.c preset.c calib_const.c)

target_link_libraries(storage INTERFACE drivers)
