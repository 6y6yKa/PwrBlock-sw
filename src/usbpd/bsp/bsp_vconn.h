/**
 ******************************************************************************
 * @file    bsp_vconn.h
 * @author  MCD Application Team
 * @brief   Header file for bsp_vconn.c
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
 * (VCONN handling split out into bsp_vconn.h)
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_pwr_def.h"

#include <stdint.h>

/**
 * @brief  Initialize VCONN sourcing.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  CCPinId Type-C CC pin identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_CC1
 *         @arg @ref USBPD_PWR_TYPE_C_CC2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VCONNInit(uint32_t Instance, uint32_t CCPinId);

/**
 * @brief  Un-Initialize VCONN sourcing.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  CCPinId Type-C CC pin identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_CC1
 *         @arg @ref USBPD_PWR_TYPE_C_CC2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VCONNDeInit(uint32_t Instance, uint32_t CCPinId);

/**
 * @brief  Enable VCONN sourcing.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  CCPinId Type-C CC pin identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_CC1
 *         @arg @ref USBPD_PWR_TYPE_C_CC2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VCONNOn(uint32_t Instance, uint32_t CCPinId);

/**
 * @brief  Disable VCONN sourcing.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  CCPinId CC pin identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_CC1
 *         @arg @ref USBPD_PWR_TYPE_C_CC2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VCONNOff(uint32_t Instance, uint32_t CCPinId);

/**
 * @brief  Get actual VCONN status.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  CCPinId Type-C CC pin identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_CC1
 *         @arg @ref USBPD_PWR_TYPE_C_CC2
 * @param  pState VCONN status (1: On, 0: Off)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VCONNIsOn(
    uint32_t Instance, uint32_t CCPinId, uint8_t* pState
);

#ifdef __cplusplus
}
#endif
