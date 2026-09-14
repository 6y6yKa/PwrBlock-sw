#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_TRACE)
// provides USBPD_TRACE_Add used in define
#include <usbpd_trace.h>  // IWYU pragma: keep
#endif

#if defined(_DEBUG_TRACE)
#define DPM_CORE_DEBUG_TRACE(_PORTNUM_, __MESSAGE__)                           \
    USBPD_TRACE_Add(                                                           \
        USBPD_TRACE_DEBUG,                                                     \
        _PORTNUM_,                                                             \
        0u,                                                                    \
        (uint8_t*)(__MESSAGE__),                                               \
        sizeof(__MESSAGE__) - 1u                                               \
    );
#else
#define DPM_CORE_DEBUG_TRACE(_PORTNUM_, __MESSAGE__)
#endif /* _DEBUG_TRACE */

#ifdef __cplusplus
}
#endif
