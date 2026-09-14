/**
 ******************************************************************************
 * @file    bsp_vbus.h
 * @author  MCD Application Team
 * @brief   Header file for bsp_vbus.c
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
 * (VBUS handling split out into bsp_vbus.h)
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_pwr_def.h"

#include <stdint.h>

/**
 * @brief  VBUS connection status
 */
#define PWR_VBUSConnectionStatusTypeDef USBPD_PWR_VBUSConnectionStatusTypeDef

/**
 * @brief  VBUS connection status
 */
typedef enum
{
    VBUS_CONNECTED = 0,
    VBUS_NOT_CONNECTED
} USBPD_PWR_VBUSConnectionStatusTypeDef;

/**
 * @brief VBUS Detection Callback
 */
typedef void USBPD_PWR_VBUSDetectCallbackFunc(
    uint32_t                              Instance,
    USBPD_PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus
);

/**
 * @brief  Standard VBUS voltage levels
 */
#define USBPD_PWR_VBUS_5V  5000U
#define USBPD_PWR_VBUS_9V  9000U
#define USBPD_PWR_VBUS_15V 15000U

/**
 * @brief  VBUS disconnection threshold values (in mV)
 */
#define USBPD_PWR_HIGH_VBUS_THRESHOLD (2800U)
#define USBPD_PWR_LOW_VBUS_THRESHOLD  (750U)
#define USBPD_PWR_VBUS_THRESHOLD_5V   (3900U)
#define USBPD_PWR_VBUS_THRESHOLD_9V   (7000U)
#define USBPD_PWR_VBUS_THRESHOLD_15V  (12500U)
#define USBPD_PWR_VBUS_THRESHOLD_20V  (17000U)
#define USBPD_PWR_VBUS_THRESHOLD_APDO (2150U)

/**
 * @brief  VBUS discharge parameters
 */
#define USBPD_PWR_DISCHARGE_MARGIN (500U)
#define USBPD_PWR_DISCHARGE_TIME   (6U)

/**
 * @brief  Initialize the hardware resources used by the Type-C power delivery
 * (PD) controller.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSInit(uint32_t Instance);

/**
 * @brief  Release the hardware resources used by the Type-C power delivery (PD)
 *         controller.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSDeInit(uint32_t Instance);

/**
 * @brief  Enable power supply over VBUS.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSOn(uint32_t Instance);

/**
 * @brief  Disable power supply over VBUS.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSOff(uint32_t Instance);

/**
 * @brief  Set a fixed/variable PDO and manage the power control.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  VbusTargetInmv the vbus Target (in mV)
 * @param  OperatingCurrent the Operating Current (in mA)
 * @param  MaxOperatingCurrent the Max Operating Current (in mA)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_Fixed(
    uint32_t Instance,
    uint32_t VbusTargetInmv,
    uint32_t OperatingCurrent,
    uint32_t MaxOperatingCurrent
);

/**
 * @brief  Set a fixed/variable PDO and manage the power control.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  VbusTargetMinInmv the vbus Target min (in mV)
 * @param  VbusTargetMaxInmv the vbus Target max (in mV)
 * @param  OperatingCurrent the Operating Current (in mA)
 * @param  MaxOperatingCurrent the Max Operating Current (in mA)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_Variable(
    uint32_t Instance,
    uint32_t VbusTargetMinInmv,
    uint32_t VbusTargetMaxInmv,
    uint32_t OperatingCurrent,
    uint32_t MaxOperatingCurrent
);

/**
 * @brief  Set a Battery PDO and manage the power control.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  VbusTargetMin the vbus Target min (in mV)
 * @param  VbusTargetMax the vbus Target max (in mV)
 * @param  OperatingPower the Operating Power (in mW)
 * @param  MaxOperatingPower the Max Operating Power (in mW)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_Battery(
    uint32_t Instance,
    uint32_t VbusTargetMin,
    uint32_t VbusTargetMax,
    uint32_t OperatingPower,
    uint32_t MaxOperatingPower
);

/**
 * @brief  Set a APDO and manage the power control.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  VbusTargetInmv the vbus Target (in mV)
 * @param  OperatingCurrent the Operating current (in mA)
 * @param  Delta Delta between with previous APDO (in mV), 0 means APDO start
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSSetVoltage_APDO(
    uint32_t Instance,
    uint32_t VbusTargetInmv,
    uint32_t OperatingCurrent,
    int32_t  Delta
);

/**
 * @brief  Get actual voltage level measured on the VBUS line.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  pVoltage Pointer on measured voltage level (in mV)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSGetVoltage(
    uint32_t Instance, uint32_t* pVoltage
);

/**
 * @brief  Get actual current level measured on the VBUS line.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  pCurrent Pointer on measured current level (in mA)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSGetCurrent(
    uint32_t Instance, int32_t* pCurrent
);

/**
 * @brief  Set the VBUS disconnection voltage threshold.
 * @note   Callback function registered through
 * BSP_USBPD_PWR_RegisterVBUSDetectCallback function call is invoked when VBUS
 * falls below programmed threshold.
 * @note   By default VBUS disconnection threshold is set to 3.3V
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  VoltageThreshold VBUS disconnection voltage threshold (in mV)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(
    uint32_t Instance, uint32_t VoltageThreshold
);

/**
 * @brief  Register USB Type-C Current callback function.
 * @note   Callback function invoked when VBUS rises above 4V (VBUS present) or
 *         when VBUS falls below programmed threshold (VBUS absent).
 * @note   Callback function is un-registered when callback function pointer
 *         argument is NULL.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  pfnVBUSDetectCallback callback function pointer
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_RegisterVBUSDetectCallback(
    uint32_t Instance, USBPD_PWR_VBUSDetectCallbackFunc* pfnVBUSDetectCallback
);

/**
 * @brief  Get actual VBUS status.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
 * @param  pState VBUS status (1: On, 0: Off)
 * @retval BSP status
 */
BspErrorTypeDef BSP_USBPD_PWR_VBUSIsOn(uint32_t Instance, uint8_t* pState);

#ifdef __cplusplus
}
#endif
