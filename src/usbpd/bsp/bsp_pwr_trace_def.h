#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_TRACE)
// provides USBPD_TRACE_Add used in define
#include <usbpd_trace.h>  // IWYU pragma: keep
#endif


/** @defgroup POWER_Private_Macros Private Macros
 * @{
 */
#if defined(_TRACE)
#define PWR_DEBUG_TRACE(_PORT_, __MESSAGE__)                                   \
    USBPD_TRACE_Add(                                                           \
        USBPD_TRACE_DEBUG,                                                     \
        (_PORT_),                                                              \
        0u,                                                                    \
        (uint8_t*)(__MESSAGE__),                                               \
        sizeof(__MESSAGE__) - 1u                                               \
    )
#else
#define PWR_DEBUG_TRACE(_PORT_, __MESSAGE__)
#endif /* _TRACE */


#ifdef __cplusplus
}
#endif
