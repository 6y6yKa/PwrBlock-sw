/**
 ******************************************************************************
 * @file    bsp_pwr.c
 * @author  MCD Application Team
 * @brief   This file contains BSP power control functions
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
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Src/usbpd_pwr_user.c
 * (renamed usbpd_pwr_user.c -> bsp_pwr.c)
 */
#include "bsp_pwr.h"

#include "bsp_pwr_trace_def.h"

#include <stm32g0xx_hal.h>

BspErrorTypeDef BSP_USBPD_PWR_Init(uint32_t Instance)
{
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_Deinit(uint32_t Instance)
{
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_Deinit");
    }

    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_SetRole(
    uint32_t Instance, USBPD_PWR_PowerRoleTypeDef Role
)
{
    (void)Role;
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_SetRole");
    }

    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_SetPowerMode(
    uint32_t Instance, USBPD_PWR_PowerModeTypeDef PwrMode
)
{
    (void)PwrMode;
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_SetPowerMode");
    }

    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_GetPowerMode(
    uint32_t Instance, const USBPD_PWR_PowerModeTypeDef* PwrMode
)
{
    (void)PwrMode;
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_GetPowerMode");
    }
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VCCSetState(uint32_t Instance, uint32_t State)
{
    (void)State;
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VCCSetState");
    }
    return ret;
}

void BSP_USBPD_PWR_EventCallback(uint32_t Instance)
{
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_EventCallback");
}
