add_library(monitor INTERFACE)
target_sources(monitor INTERFACE monitor_task.c)

target_link_libraries(monitor INTERFACE drivers)
