add_library(usbcdc INTERFACE)

add_subdirectory(usbweb)

target_sources(usbcdc INTERFACE usb_descriptors.c usbcdc_app.c)

target_link_libraries(usbcdc INTERFACE usbweb)
