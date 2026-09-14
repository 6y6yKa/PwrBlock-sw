/**
 ******************************************************************************
 * @file    usbpd_pwr_if.h
 * @author  MCD Application Team
 * @brief   Header file for usbpd_pwr_if.c
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
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Inc/usbpd_pwr_if.h
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <usbpd_def.h>

/* enumeration of the different power status available for VBUS */
typedef enum
{
    USBPD_PWR_BELOWVSAFE0V,
    USBPD_PWR_VSAFE5V,
    USBPD_PWR_SNKDETACH
} USBPD_VBUSPOWER_STATUS;

/* Enumeration of the different errors detected by power IF */
typedef enum
{
    USBPD_PWR_IF_OTHER        = 0U,
    USBPD_PWR_IF_NMI          = 2U,
    USBPD_PWR_IF_HARD_FAULT   = 3U,
    USBPD_PWR_IF_OVER_CURRENT = 4U,
} USBPD_PWR_IF_ERROR;

// Macros used to convert values into PDO representation
//! From Volt to 20mV multiples      */
#define PWR_V_20MV(_V_) ((uint16_t)(((_V_) * 1000) / 20))
//! From Volt to 50mV multiples      */
#define PWR_V_50MV(_V_) ((uint16_t)(((_V_) * 1000) / 50))
//! From Volt to 100mV multiples     */
#define PWR_V_100MV(_V_) ((uint16_t)(((_V_) * 1000) / 100))
//! From Ampere to 10mA multiples    */
#define PWR_A_10MA(_A_) ((uint16_t)(((_A_) * 1000) / 10))
//! From Ampere to 50mA multiples    */
#define PWR_A_50MA(_A_) ((uint16_t)(((_A_) * 1000) / 50))
//! From Watt to 250mW multiples     */
#define PWR_W(_W_) ((uint16_t)(((_W_) * 1000) / 250))

// Macros used to get values from PDO representation
// From 50mV multiples to mV
#define PWR_DECODE_50MV(_Value_) ((uint16_t)(((_Value_) * 50)))
// From 100mV multiples to mV
#define PWR_DECODE_100MV(_Value_) ((uint16_t)(((_Value_) * 100)))
// From 10mA multiples to mA
#define PWR_DECODE_10MA(_Value_) ((uint16_t)(((_Value_) * 10)))
// From 50mA multiples to mA
#define PWR_DECODE_50MA(_Value_) ((uint16_t)(((_Value_) * 50)))
// From 250mW multiples to mW
#define PWR_DECODE_MW(_Value_) ((uint16_t)(((_Value_) * 250)))

#define USBPD_PORT_IsValid(__Port__) ((__Port__) < (USBPD_PORT_COUNT))

/**
 * @brief  Initialize structures and variables related to power board profiles
 *         used by Sink and Source, for all available ports.
 * @retval USBPD status
 */
USBPD_StatusTypeDef USBPD_PWR_IF_Init(void);

/**
 * @brief  Checks if the power on a specified port is ready
 * @param  PortNum Port number
 * @param  Vsafe   Vsafe status based on @ref USBPD_VSAFE_StatusTypeDef
 * @retval USBPD status
 */
USBPD_StatusTypeDef USBPD_PWR_IF_SupplyReady(
    uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe
);

/**
 * @brief  Reads the voltage and the current on a specified port
 * @param  PortNum Port number
 * @param  pVoltage The Voltage in mV
 * @param  pCurrent The Current in mA
 * @retval ENABLE or DISABLE
 */
USBPD_StatusTypeDef USBPD_PWR_IF_ReadVA(
    uint8_t PortNum, uint16_t* pVoltage, uint16_t* pCurrent
);

/**
 * @brief  Enables the VConn on the port.
 * @param  PortNum Port number
 * @param  CCpin Specifies the CCx to be selected
 * based on @ref CCxPin_TypeDef structure
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_PWR_IF_Enable_VConn(
    uint8_t PortNum, CCxPin_TypeDef CCpin
);

/**
 * @brief  Disable the VConn on the port.
 * @param  PortNum Port number
 * @param  CCpin Specifies the CCx to be selected
 * based on @ref CCxPin_TypeDef structure
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_PWR_IF_Disable_VConn(
    uint8_t PortNum, CCxPin_TypeDef CCpin
);

/**
 * @brief  Allow PDO data reading from PWR_IF storage.
 * @param  PortNum Port number
 * @param  DataId Type of data to be read from PWR_IF
 *         This parameter can be one of the following values:
 *           @arg @ref USBPD_CORE_DATATYPE_SRC_PDO Source PDO reading requested
 *           @arg @ref USBPD_CORE_DATATYPE_SNK_PDO Sink PDO reading requested
 * @param  Ptr Pointer on address where PDO values should be written (u8
 * pointer)
 * @param  Size Pointer on nb of u32 written by PWR_IF (nb of PDOs)
 * @retval None
 */
void USBPD_PWR_IF_GetPortPDOs(
    uint8_t                         PortNum,
    USBPD_CORE_DataInfoType_TypeDef DataId,
    uint8_t*                        Ptr,
    uint32_t*                       Size
);

/**
 * @brief  Find out SRC PDO pointed out by a position provided in a Request DO
 * (from Sink).
 * @param  PortNum Port number
 * @param  RdoPosition RDO Position in list of provided PDO
 * @param  Pdo Pointer on PDO value pointed out by RDO position (u32 pointer)
 * @retval Status of search
 *         USBPD_OK : Src PDO found for requested DO position (output Pdo
 * parameter is set) USBPD_FAIL : Position is not compliant with current Src PDO
 * for this port (no corresponding PDO value)
 */
USBPD_StatusTypeDef USBPD_PWR_IF_SearchRequestedPDO(
    uint8_t PortNum, uint32_t RdoPosition, uint32_t* Pdo
);

/**
 * @brief  Function called in case of critical issue is detected to switch in
 * safety mode.
 * @param  ErrorType Type of error detected by monitoring (based on @ref
 * USBPD_PWR_IF_ERROR)
 * @retval None
 */
void USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_ERROR ErrorType);

/**
 * @brief  Function called in case of critical issue is detected to switch in
 * safety mode.
 * @retval None
 */
void USBPD_PWR_IF_Alarm(void);

/**
 * @brief Function is called to get VBUS power status.
 * @param PortNum Port number
 * @param PowerTypeStatus  Power type status based on @ref
 * USBPD_VBUSPOWER_STATUS
 * @retval UBBPD_TRUE or USBPD_FALSE
 */
uint8_t USBPD_PWR_IF_GetVBUSStatus(
    uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus
);

/**
 * @brief Function is called to set the VBUS threshold when a request has been
 * accepted.
 * @param PortNum Port number
 * @retval None
 */
void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum);

/**
 * @brief Function is called to reset the VBUS threshold when there is a power
 * reset.
 * @param PortNum Port number
 * @retval None
 */
void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum);

#ifdef __cplusplus
}
#endif
