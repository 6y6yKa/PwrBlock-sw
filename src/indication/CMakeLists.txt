add_library(indication INTERFACE)
target_sources(indication INTERFACE indication.c indication_task.c)

target_link_libraries(indication INTERFACE drivers)
