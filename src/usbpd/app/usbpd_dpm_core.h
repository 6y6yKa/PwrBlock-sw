/**
 ******************************************************************************
 * @file    usbpd_dpm_core.h
 * @author  MCD Application Team
 * @brief   Header file for usbpd_dpm_core.c
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
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Inc/usbpd_dpm_core.h
 */
#pragma once

#include "usbpd_def.h"

#ifdef __cplusplus
extern "C" {
#endif

extern USBPD_ParamsTypeDef DPM_Params[USBPD_PORT_COUNT];

/**
 * @brief  Initialize the core stack (port power role, PWR_IF, CAD and PE Init
 * procedures)
 * @retval USBPD status
 */
USBPD_StatusTypeDef USBPD_DPM_InitCore(void);

/**
 * @brief  Initialize the OS parts (task, queue,... )
 * @retval USBPD status
 */
USBPD_StatusTypeDef USBPD_DPM_InitOS(void);

/**
 * @brief  RTOS system timer handler
 */
void USBPD_DPM_TimerCounter(void);

/**
 * @brief DPM Core Error Handler called from PE or CAD FSM
 */
__WEAK void USBPD_DPM_ErrorHandler(void);

#ifdef __cplusplus
}
#endif
