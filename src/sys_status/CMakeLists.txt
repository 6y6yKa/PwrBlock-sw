add_library(sys_status INTERFACE)
target_sources(sys_status INTERFACE sys_status_task.c)

target_link_libraries(sys_status INTERFACE drivers)
