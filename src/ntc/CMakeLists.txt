add_library(ntc INTERFACE)
target_sources(ntc INTERFACE ntc.c)

target_link_libraries(ntc INTERFACE middleware m)
