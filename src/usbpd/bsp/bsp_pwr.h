/**
 ******************************************************************************
 * @file    bsp_pwr.h
 * @author  MCD Application Team
 * @brief   Header file for bsp_pwr.c
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
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Inc/usbpd_pwr_user.h
 * (renamed usbpd_pwr_user.h -> bsp_pwr.h)
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_pwr_def.h"

#include <stdint.h>

/**
 * @brief  Power role
 */
typedef enum
{
    POWER_ROLE_SOURCE = 0,
    POWER_ROLE_SINK,
    POWER_ROLE_DUAL
} USBPD_PWR_PowerRoleTypeDef;

/**
 * @brief  Voltage control mode
 */
typedef enum
{
    DCDC_CTRL_MODE_UNKNOWN = 0,
    DCDC_CTRL_MODE_GPIO,
    DCDC_CTRL_MODE_PWM,
} USBPD_PWR_DCDCCtrlModeTypeDef;

/**
 * @brief  Low Power Mode of TypeC ports
 */
typedef enum
{
    USBPD_PWR_MODE_OFF = 0,
    USBPD_PWR_MODE_HIBERNATE,
    USBPD_PWR_MODE_LOWPOWER,
    USBPD_PWR_MODE_NORMAL
} USBPD_PWR_PowerModeTypeDef;

/**
 * @brief  Global initialization of PWR resource used by USB-PD
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_Init(uint32_t Instance);

/**
 * @brief  Global de-initialization of PWR resource used by USB-PD
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_Deinit(uint32_t Instance);

/**
 * @brief  Assign Power role for current Port (Source or Sink)
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 * @param  Role Type-C port role
 *         This parameter can be take one of the following values:
 *         @arg @ref POWER_ROLE_SOURCE
 *         @arg @ref POWER_ROLE_SINK
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_SetRole(
    uint32_t Instance, USBPD_PWR_PowerRoleTypeDef Role
);

/**
 * @brief  Set operating mode of Port regarding power saving constraints
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  PwrMode Type-C port power saving mode
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_MODE_OFF
 *         @arg @ref USBPD_PWR_MODE_HIBERNATE
 *         @arg @ref USBPD_PWR_MODE_LOWPOWER
 *         @arg @ref USBPD_PWR_MODE_NORMAL
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_SetPowerMode(
    uint32_t Instance, USBPD_PWR_PowerModeTypeDef PwrMode
);

/**
 * @brief  Get operating mode of Port regarding power saving constraints
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  PwrMode Pointer on current Type-C port power saving mode value
 *         Following values are available :
 *         @arg @ref USBPD_PWR_MODE_OFF
 *         @arg @ref USBPD_PWR_MODE_HIBERNATE
 *         @arg @ref USBPD_PWR_MODE_LOWPOWER
 *         @arg @ref USBPD_PWR_MODE_NORMAL
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_GetPowerMode(
    uint32_t Instance, const USBPD_PWR_PowerModeTypeDef* PwrMode
);

/**
 * @brief  Set state of VCC (specific functions)
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  State VCC state
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VCCSetState(uint32_t Instance, uint32_t State);

/**
 * @brief  USBPD PWR callback used to notify a asynchronous PWR event.
 *         (This callback caould be called fromp an IT routine, associated to
 *         any PWR related event detection mechanism)
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval None
 */
void BSP_USBPD_PWR_EventCallback(uint32_t Instance);

#ifdef __cplusplus
}
#endif
