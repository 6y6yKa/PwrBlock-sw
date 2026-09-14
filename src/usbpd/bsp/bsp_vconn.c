/**
 ******************************************************************************
 * @file    bsp_vconn.c
 * @author  MCD Application Team
 * @brief   This file contains BSP VCONN control functions
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
 * (VCONN handling split out into bsp_vconn.c)
 */
#include "bsp_vconn.h"

#include "bsp_pwr_def.h"
#include "bsp_pwr_trace_def.h"

#include <stm32g0xx_hal.h>

BspErrorTypeDef BSP_USBPD_PWR_VCONNInit(uint32_t Instance, uint32_t CCPinId)
{
    (void)CCPinId;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VCONNInit");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VCONNDeInit(uint32_t Instance, uint32_t CCPinId)
{
    (void)CCPinId;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VCONNDeInit");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VCONNOn(uint32_t Instance, uint32_t CCPinId)
{
    (void)CCPinId;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VCONNOn");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VCONNOff(uint32_t Instance, uint32_t CCPinId)
{
    (void)CCPinId;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VCONNOff");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VCONNIsOn(
    uint32_t Instance, uint32_t CCPinId, uint8_t* pState
)
{
    (void)CCPinId;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    *pState = 0U;

    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VCONNIsOn");
    return ret;
}
