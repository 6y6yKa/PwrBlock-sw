/**
 ******************************************************************************
 * @file    bsp_vbus.c
 * @author  MCD Application Team
 * @brief   This file contains BSP VBUS control functions
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
 * (VBUS handling split out into bsp_vbus.c)
 */
#include "bsp_vbus.h"

#include "bsp_pwr_trace_def.h"

#include <stm32g0xx_hal.h>

BspErrorTypeDef BSP_USBPD_PWR_VBUSInit(uint32_t Instance)
{
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSInit");
    }

    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSDeInit(uint32_t Instance)
{
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSDeInit");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSOn(uint32_t Instance)
{
    /* Check if instance is valid       */
    int32_t ret;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSOn");
    }
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSOff(uint32_t Instance)
{
    /* Check if instance is valid       */
    int32_t ret;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSOff");
    }
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_Fixed(
    uint32_t Instance,
    uint32_t VbusTargetInmv,
    uint32_t OperatingCurrent,
    uint32_t MaxOperatingCurrent
)
{
    (void)VbusTargetInmv;
    (void)OperatingCurrent;
    (void)MaxOperatingCurrent;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(
        Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSSetVoltage_Fixed"
    );
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_Variable(
    uint32_t Instance,
    uint32_t VbusTargetMinInmv,
    uint32_t VbusTargetMaxInmv,
    uint32_t OperatingCurrent,
    uint32_t MaxOperatingCurrent
)
{
    (void)VbusTargetMinInmv;
    (void)VbusTargetMaxInmv;
    (void)OperatingCurrent;
    (void)MaxOperatingCurrent;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(
        Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSSetVoltage_Variable"
    );
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_Battery(
    uint32_t Instance,
    uint32_t VbusTargetMin,
    uint32_t VbusTargetMax,
    uint32_t OperatingPower,
    uint32_t MaxOperatingPower
)
{
    (void)VbusTargetMin;
    (void)VbusTargetMax;
    (void)OperatingPower;
    (void)MaxOperatingPower;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(
        Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSSetVoltage_Battery"
    );
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_APDO(
    uint32_t Instance,
    uint32_t VbusTargetInmv,
    uint32_t OperatingCurrent,
    int32_t  Delta
)
{
    (void)VbusTargetInmv;
    (void)OperatingCurrent;
    (void)Delta;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(
        Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSSetVoltage_APDO"
    );
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSGetVoltage(
    uint32_t Instance, uint32_t* pVoltage
)
{
    // TODO: Can power block measure input voltage from SNK?
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_NONE;

    if (pVoltage == NULL) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret       = BSP_ERROR_WRONG_PARAM;
        *pVoltage = 0;
    }
    else {
        *pVoltage = 0;
    }

    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSGetVoltage");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSGetCurrent(
    uint32_t Instance, int32_t* pCurrent
)
{
    /* Check if instance is valid       */
    int32_t ret;

    if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent)) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        *pCurrent = 0;
        ret       = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }
    PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSGetCurrent");
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(
    uint32_t Instance, uint32_t VoltageThreshold
)
{
    (void)VoltageThreshold;

    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(
        Instance, "ADVICE: Update BSP_USBPD_PWR_SetVBUSDisconnectionThreshold"
    );
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_RegisterVBUSDetectCallback(
    uint32_t Instance, USBPD_PWR_VBUSDetectCallbackFunc* pfnVBUSDetectCallback
)
{
    (void)pfnVBUSDetectCallback;
    /* Check if instance is valid       */
    int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    PWR_DEBUG_TRACE(
        Instance, "ADVICE: Update BSP_USBPD_PWR_RegisterVBUSDetectCallback"
    );
    return ret;
}

BspErrorTypeDef BSP_USBPD_PWR_VBUSIsOn(uint32_t Instance, uint8_t* pState)
{
    /* Check if instance is valid       */
    int32_t ret;

    if (Instance >= USBPD_PWR_INSTANCES_NBR) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    else {
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        PWR_DEBUG_TRACE(Instance, "ADVICE: Update BSP_USBPD_PWR_VBUSIsOn");
    }
    *pState = 0U;
    return ret;
}
