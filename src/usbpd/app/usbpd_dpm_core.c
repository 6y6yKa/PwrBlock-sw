/**
 ******************************************************************************
 * @file    usbpd_dpm_core.c
 * @author  MCD Application Team
 * @brief   USBPD dpm core file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 *
 * Adapted from STMicroelectronics x-cube-tcpp,
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Src/usbpd_dpm_core.c
 */

// TODO: Fix private scope implementation in dpm_conf.h
#define USBPD_DPM_CORE_C

#include "usbpd_dpm_core.h"

#include "dpm/dpm.h"
#include "dpm/dpm_conf.h"
#include "threads/threads_def.h"
#include "usbpd_core.h"
#include "usbpd_os_port.h"

#include "usbpd_dpm_trace_def.h"  // IWYU pragma: keep for debug purpose

DEF_TASK_FUNCTION(USBPD_TaskUser);
DEF_TASK_FUNCTION(USBPD_CAD_Task);
DEF_TASK_FUNCTION(USBPD_PE_CableTask);
DEF_TASK_FUNCTION(USBPD_PE_Task);

#define CHECK_PE_FUNCTION_CALL(_function_)                                     \
    do {                                                                       \
        _retr = _function_;                                                    \
        if (USBPD_OK != _retr) {                                               \
            goto error;                                                        \
        }                                                                      \
    } while (0);

#define CHECK_CAD_FUNCTION_CALL(_function_)                                    \
    if (USBPD_CAD_OK != (_function_)) {                                        \
        _retr = USBPD_ERROR;                                                   \
        goto error;                                                            \
    }

/* Private variables ---------------------------------------------------------*/
static OS_TASK_ID  DPM_PEThreadId_Table[USBPD_PORT_COUNT];
static OS_QUEUE_ID CADQueueId;
static OS_TASK_ID  CADThread;
static OS_QUEUE_ID PEQueueId[USBPD_PORT_COUNT];

/* Public variables ---------------------------------------------------------*/
USBPD_ParamsTypeDef DPM_Params[USBPD_PORT_COUNT];

/* Private function prototypes -----------------------------------------------*/
// TODO: Part of DPM interface for PE, CAD outside of DPM component.
// Should provide RTOS object to manage instead of functions?
static void USBPD_PE_TaskWakeUp(uint8_t PortNum);
static void DPM_StartPETask(uint8_t PortNum);
void        USBPD_DPM_CADCallback(
           uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef CCpin
       );
static void USBPD_DPM_CADTaskWakeUp(void);

// TODO: DPM initialization should be described in DPM component
USBPD_StatusTypeDef USBPD_DPM_InitCore(void)
{
    /* variable to get dynamique memory allocated by usbpd stack */
    uint32_t            stack_dynamemsize;
    USBPD_StatusTypeDef _retr = USBPD_OK;

    /* CAD callback definition */
    static const USBPD_PE_Callbacks dpmCallbacks = {
        NULL,
        USBPD_DPM_HardReset,
        NULL,
        USBPD_DPM_Notification,
        USBPD_DPM_ExtendedMessageReceived,
        USBPD_DPM_GetDataInfo,
        USBPD_DPM_SetDataInfo,
        NULL,
        USBPD_DPM_SNK_EvaluateCapabilities,
        NULL,
        USBPD_PE_TaskWakeUp,
        NULL,
        NULL,
        USBPD_DPM_EnterErrorRecovery,
        USBPD_DPM_EvaluateDataRoleSwap,
        USBPD_DPM_IsPowerReady,
    };

    static const USBPD_CAD_Callbacks CAD_cbs = {
        USBPD_DPM_CADCallback,
        USBPD_DPM_CADTaskWakeUp,
    };

    /* Check the lib selected */
    if (USBPD_TRUE != USBPD_PE_CheckLIB(LIB_ID)) {
        _retr = USBPD_ERROR;
        goto error;
    }

    /* to get how much memory are dynamically allocated by the stack
       the memory return is corresponding to 2 ports so if the application
       managed only one port divide the value return by 2                   */
    stack_dynamemsize = USBPD_PE_GetMemoryConsumption();

    /* done to avoid warning */
    (void)stack_dynamemsize;

#if defined(_TRACE) || defined(_GUI_INTERFACE)
    /* Initialise the TRACE */
    USBPD_TRACE_Init();
#endif /* _TRACE || _GUI_INTERFACE */

    for (uint8_t _port_index = 0; _port_index < USBPD_PORT_COUNT;
         ++_port_index) {
        /* Variable to be sure that DPM is correctly initialized */
        DPM_Params[_port_index].DPM_Initialized = USBPD_FALSE;

        /* check the stack settings */
        DPM_Params[_port_index].PE_SpecRevision =
            DPM_Settings[_port_index].PE_SpecRevision;
        DPM_Params[_port_index].PE_PowerRole =
            DPM_Settings[_port_index].PE_DefaultRole;
        DPM_Params[_port_index].PE_SwapOngoing = USBPD_FALSE;
        DPM_Params[_port_index].ActiveCCIs     = CCNONE;
        DPM_Params[_port_index].VconnCCIs      = CCNONE;
        DPM_Params[_port_index].VconnStatus    = USBPD_FALSE;

        /* CAD SET UP : Port 0 */
        CHECK_CAD_FUNCTION_CALL(USBPD_CAD_Init(
            _port_index,
            &CAD_cbs,
            &DPM_Settings[_port_index],
            &DPM_Params[_port_index]
        ));

        /* PE SET UP : Port 0 */
        CHECK_PE_FUNCTION_CALL(USBPD_PE_Init(
            _port_index,
            (USBPD_SettingsTypeDef*)&DPM_Settings[_port_index],
            &DPM_Params[_port_index],
            &dpmCallbacks
        ));

        /* DPM is correctly initialized */
        DPM_Params[_port_index].DPM_Initialized = USBPD_TRUE;

        /* Enable CAD on Port 0 */
        USBPD_CAD_PortEnable(_port_index, USBPD_CAD_ENABLE);
    }
error:
    return _retr;
}

USBPD_StatusTypeDef USBPD_DPM_InitOS(void)
{
    OS_INIT();
    {
        OS_CREATE_QUEUE(CADQueueId, "QCAD", USBPD_PORT_COUNT, OS_ELEMENT_SIZE);
        OS_DEFINE_TASK(
            CAD, USBPD_CAD_Task, OS_CAD_PRIORITY, OS_CAD_STACK_SIZE, NULL
        );
        OS_CREATE_TASK(
            CADThread,
            CAD,
            USBPD_CAD_Task,
            OS_CAD_PRIORITY,
            OS_CAD_STACK_SIZE,
            NULL
        );
    }
    /* Create the queue corresponding to PE task */
    for (uint32_t index = 0; index < USBPD_PORT_COUNT; index++) {

        if (index == USBPD_PORT_0) {
            /* Create the queue corresponding to PE task */
            OS_CREATE_QUEUE(PEQueueId[index], QPE_0, 1, OS_ELEMENT_SIZE);
            /* Tasks definition */
            OS_DEFINE_TASK(
                PE_0,
                USBPD_PE_Task,
                OS_PE_PRIORITY,
                OS_PE_STACK_SIZE,
                USBPD_PORT_0
            );
            OS_CREATE_TASK(
                DPM_PEThreadId_Table[USBPD_PORT_0],
                PE_0,
                USBPD_PE_Task,
                OS_PE_PRIORITY,
                OS_PE_STACK_SIZE,
                NULL
            );
        }
    }
error:
    return _retr;
}

void USBPD_DPM_TimerCounter(void)
{
    /* Call PE/PRL timers functions only if DPM is initialized */
    if (USBPD_TRUE == DPM_Params[USBPD_PORT_0].DPM_Initialized) {
        USBPD_PE_TimerCounter(USBPD_PORT_0);
        USBPD_PRL_TimerCounter(USBPD_PORT_0);
    }
}

/**
 * @brief  WakeUp PE task
 * @param  PortNum port number
 * @retval None
 */
static void USBPD_PE_TaskWakeUp(uint8_t PortNum)
{
    OS_PUT_MESSAGE_QUEUE(PEQueueId[PortNum], 0xFFFFU, 0U);
}

/**
 * @brief  WakeUp CAD task
 * @retval None
 */
static void USBPD_DPM_CADTaskWakeUp(void)
{
    OS_PUT_MESSAGE_QUEUE(CADQueueId, 0xFFFF, 0);
}

/**
 * @brief  Main task for PE layer
 * @param  argument Not used
 * @retval None
 */
DEF_TASK_FUNCTION(USBPD_PE_Task)
{
    uint8_t  _port = (uint32_t)argument;
    uint32_t _timing;
    for (;;) {
        if (DPM_Params[_port].PE_IsConnected == USBPD_FALSE) {
            /* if the port is no more connected, suspend the PE thread */
            OS_TASK_SUSPEND(OS_TASK_GETID());
        }

#if defined(_SNK)
        _timing = USBPD_PE_StateMachine_SNK(_port);
#else
#error "a type of state machine should be supported"
#endif /* _SNK */

        OS_GETMESSAGE_QUEUE(PEQueueId[_port], _timing);
    }
}

/**
 * @brief  Main task for CAD layer
 * @param  argument Not used
 * @retval None
 */
DEF_TASK_FUNCTION(USBPD_CAD_Task)
{
    (void)argument;
    uint32_t _timing;
    for (;;) {
        _timing = USBPD_CAD_Process();
        OS_GETMESSAGE_QUEUE(CADQueueId, _timing);
    }
}

/**
 * @brief  CallBack reporting events on a specified port from CAD layer.
 * @param  PortNum   The handle of the port
 * @param  State     CAD state
 * @param  Cc        The Communication Channel for the USBPD communication
 * @retval None
 */
void USBPD_DPM_CADCallback(
    uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef CCpin
)
{
#ifdef _TRACE
    USBPD_TRACE_Add(USBPD_TRACE_CADEVENT, PortNum, (uint8_t)State, NULL, 0);
#endif /* _TRACE */
    (void)(CCpin);
    switch (State) {

        case USBPD_CAD_EVENT_ATTEMC: {
            USBPD_DPM_CableDetection(PortNum, USBPD_CAD_EVENT_ATTEMC);
            DPM_StartPETask(PortNum);
            break;
        }
        case USBPD_CAD_EVENT_ATTACHED:
            USBPD_DPM_CableDetection(PortNum, USBPD_CAD_EVENT_ATTACHED);
            DPM_StartPETask(PortNum);
            break;

        case USBPD_CAD_EVENT_DETACHED:
        case USBPD_CAD_EVENT_EMC: {
            /* Terminate PE task */
            uint8_t _timeout = 0;
            /* WakeUp PE task to let him enter suspend mode */
            USBPD_PE_TaskWakeUp(PortNum);
            /* Wait PE Let time to PE to complete the ongoing action */
            while (!OS_TASK_IS_SUSPENDED(DPM_PEThreadId_Table[PortNum])) {
                (void)OS_DELAY(1U);
                _timeout++;
                if (_timeout > 30U) {
                    /* Kill the PE task */
                    (void)OS_TASK_KILL(DPM_PEThreadId_Table[PortNum]);
                    DPM_PEThreadId_Table[PortNum] = 0;
                    break;
                }
            };
            /* Stop the PE state machine */
            USBPD_PE_StateMachine_Stop(PortNum);
            DPM_Params[PortNum].PE_SwapOngoing = USBPD_FALSE;
            DPM_Params[PortNum].PE_Power       = USBPD_POWER_NO;
            USBPD_DPM_CableDetection(PortNum, State);
            break;
        }
        default:
            /* nothing to do */
            break;
    }
}

static void DPM_StartPETask(uint8_t PortNum)
{
    USBPD_PE_StateMachine_Reset(PortNum);
    USBPD_StatusTypeDef _retr =
        USBPD_OK; /* Added for comptibility with the macro OS_CreateTask */
    /* Resume the PE task */
    switch (PortNum) {
        case USBPD_PORT_0:
        case USBPD_PORT_1: {
            if (DPM_PEThreadId_Table[PortNum] != 0) {
                OS_TASK_RESUME(DPM_PEThreadId_Table[PortNum]);
            }
            else {
                OS_CREATE_TASK(
                    DPM_PEThreadId_Table[PortNum],
                    PE_0,
                    USBPD_PE_Task,
                    OS_PE_PRIORITY,
                    OS_PE_STACK_SIZE,
                    NULL
                );
            }
            break;
        }
        default: {
            USBPD_DPM_ErrorHandler();
            break;
        }
    }
error:
    (void)_retr;
}

__WEAK void USBPD_DPM_ErrorHandler(void)
{
    /* This function is called to block application execution
       in case of an unexpected behavior
       another solution could be to reset application */
    while (1) {};
}
