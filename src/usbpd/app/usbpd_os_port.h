#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_RTOS)
#include <cmsis_os2.h>
#if (osCMSIS < 0x20000U)
#error "Target RTOS is not supported"
#endif /* osCMSIS < 0x20000U */
#else
#error "RTOS is required"
#endif /* _RTOS */

/* Exported define -----------------------------------------------------------*/
/**
 * @brief macro definition used to define the task function
 */
#define DEF_TASK_FUNCTION(__FUNCTION__) void(__FUNCTION__)(void* argument)


/**
 * @brief macro definition used to initialize the OS environment
 */
#define OS_INIT() USBPD_StatusTypeDef _retr = USBPD_OK;

/**
 * @brief macro definition the define a queue type
 */
#define OS_QUEUE_ID osMessageQueueId_t


/**
 * @brief macro definition the define a queue type
 */
#define OS_ELEMENT_SIZE sizeof(uint16_t)

/**
 * @brief macro definition used to define a queue
 */
#define OS_CREATE_QUEUE(_ID_, _NAME_, _ELT_, _ELTSIZE_)                        \
    do {                                                                       \
        (_ID_) = osMessageQueueNew((_ELT_), (_ELTSIZE_), NULL);                \
    } while (0)

/**
 * @brief macro definition used to read a queue message
 */
#define OS_GETMESSAGE_QUEUE(_ID_, _TIME_)                                      \
    do {                                                                       \
        uint32_t event;                                                        \
        (void)osMessageQueueGet((_ID_), &event, NULL, (_TIME_));               \
    } while (0)

/**
 * @brief macro definition used to define put a message inside the queue
 */
#define OS_PUT_MESSAGE_QUEUE(_ID_, _MSG_, _TIMEOUT_)                           \
    do {                                                                       \
        uint32_t event = (_MSG_);                                              \
        (void)osMessageQueuePut((_ID_), &event, 0U, (_TIMEOUT_));              \
    } while (0)

/**
 * @brief macro definition used to define a task
 */
#define OS_DEFINE_TASK(_NAME_, _FUNC_, _PRIORITY_, _STACK_SIZE_, _PARAM_)

/**
 * @brief macro definition of the TASK id
 */
#define OS_TASK_ID osThreadId_t

/**
 * @brief macro definition used to create a task
 */
#define OS_CREATE_TASK(                                                        \
    _ID_, _NAME_, _FUNC_, _PRIORITY_, _STACK_SIZE_, _PARAM_                    \
)                                                                              \
    do {                                                                       \
        osThreadAttr_t Thread_Atrr = {                                         \
            .name       = #_NAME_,                                             \
            .priority   = (_PRIORITY_),                                        \
            .stack_size = (_STACK_SIZE_)                                       \
        };                                                                     \
        (_ID_) = osThreadNew(_FUNC_, (void*)(_PARAM_), &Thread_Atrr);          \
        if (NULL == (_ID_)) {                                                  \
            _retr = USBPD_ERROR;                                               \
            goto error;                                                        \
        }                                                                      \
    } while (0)

/* Legacy define for typo error */
#define OS_TASK_IS_SUPENDED OS_TASK_IS_SUSPENDED

/**
 * @brief macro definition used to check is task is suspended
 */
#define OS_TASK_IS_SUSPENDED(_ID_) (osThreadBlocked == osThreadGetState((_ID_)))

/**
 * @brief macro definition used to get the task ID
 */
#define OS_TASK_GETID() osThreadGetId()

/**
 * @brief macro definition used to suspend a task
 */
#define OS_TASK_SUSPEND(_ID_) osThreadSuspend(_ID_)

/**
 * @brief macro definition used to kill a task
 */
#define OS_TASK_KILL(_ID_) osThreadTerminate(_ID_)

/**
 * @brief macro definition used to resume a task
 */
#define OS_TASK_RESUME(_ID_) osThreadResume(_ID_)

/**
 * @brief macro definition used to manage the delay
 */
#define OS_DELAY(_TIME_) osDelay(_TIME_)

/**
 * @brief macro definition used to start the task scheduling
 */
#define OS_KERNEL_START()                                                      \
    do {                                                                       \
        (void)osKernelInitialize();                                            \
        (void)osKernelStart();                                                 \
    } while (0)

#ifdef __cplusplus
}
#endif
