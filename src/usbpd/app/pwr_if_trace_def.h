#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_TRACE)
// provides USBPD_TRACE_Add used in define
#include <usbpd_trace.h>  // IWYU pragma: keep
#endif

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Macros
 * @{
 */
#if defined(_TRACE)
#define PWR_IF_DEBUG_TRACE(_PORT_, __MESSAGE__)                                \
    USBPD_TRACE_Add(                                                           \
        USBPD_TRACE_DEBUG,                                                     \
        (_PORT_),                                                              \
        0u,                                                                    \
        (uint8_t*)(__MESSAGE__),                                               \
        sizeof(__MESSAGE__) - 1u                                               \
    )
#else
#define PWR_IF_DEBUG_TRACE(_PORT_, __MESSAGE__)
#endif /* _TRACE */

#ifdef __cplusplus
}
#endif
