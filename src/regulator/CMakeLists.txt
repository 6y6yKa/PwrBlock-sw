add_library(regulator INTERFACE)
target_sources(regulator INTERFACE regulator.c regulator_task.c)

target_link_libraries(regulator INTERFACE drivers)
