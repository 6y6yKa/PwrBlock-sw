#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_TRACE)
// provides USBPD_TRACE_Add used in define
#include <string.h>       // IWYU pragma: keep
#include <usbpd_trace.h>  // IWYU pragma: keep
#endif

/** @defgroup USBPD_USER_PRIVATE_MACROS USBPD USER Private Macros
 * @{
 */
#if defined(_TRACE)
#define DPM_USER_DEBUG_TRACE_SIZE 50u
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)                                      \
    do {                                                                       \
        char    _str[DPM_USER_DEBUG_TRACE_SIZE];                               \
        uint8_t _size =                                                        \
            snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);            \
        if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                 \
            USBPD_TRACE_Add(                                                   \
                USBPD_TRACE_DEBUG,                                             \
                (uint8_t)(_PORT_),                                             \
                0,                                                             \
                (uint8_t*)_str,                                                \
                strlen(_str)                                                   \
            );                                                                 \
        else                                                                   \
            USBPD_TRACE_Add(                                                   \
                USBPD_TRACE_DEBUG,                                             \
                (uint8_t)(_PORT_),                                             \
                0,                                                             \
                (uint8_t*)_str,                                                \
                DPM_USER_DEBUG_TRACE_SIZE                                      \
            );                                                                 \
    } while (0)

#define DPM_USER_ERROR_TRACE(_PORT_, _STATUS_, ...)                            \
    do {                                                                       \
        if (USBPD_OK != _STATUS_) {                                            \
            char    _str[DPM_USER_DEBUG_TRACE_SIZE];                           \
            uint8_t _size =                                                    \
                snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);        \
            if (_size < DPM_USER_DEBUG_TRACE_SIZE)                             \
                USBPD_TRACE_Add(                                               \
                    USBPD_TRACE_DEBUG,                                         \
                    (uint8_t)(_PORT_),                                         \
                    0,                                                         \
                    (uint8_t*)_str,                                            \
                    strlen(_str)                                               \
                );                                                             \
            else                                                               \
                USBPD_TRACE_Add(                                               \
                    USBPD_TRACE_DEBUG,                                         \
                    (uint8_t)(_PORT_),                                         \
                    0,                                                         \
                    (uint8_t*)_str,                                            \
                    DPM_USER_DEBUG_TRACE_SIZE                                  \
                );                                                             \
        }                                                                      \
    } while (0)
#else
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)
#define DPM_USER_ERROR_TRACE(_PORT_, _STATUS_, ...)
#endif /* _TRACE */

#ifdef __cplusplus
}
#endif
