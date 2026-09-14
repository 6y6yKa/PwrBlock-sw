/**
 ******************************************************************************
 * @file    bsp_pwr_def.h
 * @author  MCD Application Team
 * @brief   Common BSP power type/constant definitions
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
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup POWER_Exported_Constants2 Exported Constants (BSP V2)
 * @{
 */
/* Common Error codes */
typedef enum
{
    BSP_ERROR_NONE                  = 0,
    BSP_ERROR_NO_INIT               = -1,
    BSP_ERROR_WRONG_PARAM           = -2,
    BSP_ERROR_BUSY                  = -3,
    BSP_ERROR_PERIPH_FAILURE        = -4,
    BSP_ERROR_COMPONENT_FAILURE     = -5,
    BSP_ERROR_UNKNOWN_FAILURE       = -6,
    BSP_ERROR_UNKNOWN_COMPONENT     = -7,
    BSP_ERROR_BUS_FAILURE           = -8,
    BSP_ERROR_CLOCK_FAILURE         = -9,
    BSP_ERROR_MSP_FAILURE           = -10,
    BSP_ERROR_FEATURE_NOT_SUPPORTED = -11,
} BspErrorTypeDef;

/**
 * @brief  Number of TypeC ports
 */
#define USBPD_PWR_INSTANCES_NBR (2U)

/**
 * @brief  Type-C port identifier
 */
#define USBPD_PWR_TYPE_C_PORT_1 (0U)
#define USBPD_PWR_TYPE_C_PORT_2 (1U)

/**
 * @brief  CC pin identifier
 */
#define USBPD_PWR_TYPE_C_CC1 (1U)
#define USBPD_PWR_TYPE_C_CC2 (2U)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
