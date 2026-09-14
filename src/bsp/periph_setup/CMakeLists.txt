add_library(periph_setup INTERFACE)

target_sources(
    periph_setup
    INTERFACE Src/main.c
              Src/stm32g0xx_hal_msp.c
              Src/stm32g0xx_hal_timebase_tim.c
              Src/stm32g0xx_it.c
              Src/syscalls.c
              Src/sysmem.c
              Src/system_stm32g0xx.c
              startup_stm32g0b1xx.s
)

target_include_directories(periph_setup INTERFACE Inc)
