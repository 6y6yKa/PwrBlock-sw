add_library(scpi INTERFACE)
target_sources(scpi INTERFACE scpi_def.c)
target_link_libraries(scpi INTERFACE scpi_parser)
