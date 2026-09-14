/**
 ******************************************************************************
 * @file    usbpd_pwr_if.c
 * @author  MCD Application Team
 * @brief   This file contains power interface control functions
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
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Src/usbpd_pwr_if.c
 */
#include "usbpd_pwr_if.h"

#include <string.h>
#include <usbpd_def.h>

#include "bsp/bsp_vbus.h"
#include "pwr_if_trace_def.h"  // IWYU pragma: keep for debug purpose

//! Number of Source PDOs (applicable for port 0)
#define PORT0_NB_SOURCEPDO 0U
//! Number of Sink PDOs (applicable for port 0)
#define PORT0_NB_SINKPDO 3U
//! Number of Source PDOs (applicable for port 1)
#define PORT1_NB_SOURCEPDO 0U
//! Number of Sink PDOs (applicable for port 1)
#define PORT1_NB_SINKPDO 0U

// NOLINTBEGIN(misc-redundant-expression)
const uint32_t PDO_FIXED_BASE = USBPD_PDO_TYPE_FIXED |
                                USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED |
                                USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED |
                                USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE |
                                USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED |
                                USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED |
                                USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED |
                                // USBPD_PDO_SRC_FIXED_EPR_NOT_SUPPORTED |
                                USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL;

const uint32_t PDO_FIXED_20V_45W =
    PDO_FIXED_BASE | (PWR_V_50MV(20) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos) |
    (PWR_A_10MA(2.25) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos);

const uint32_t PDO_FIXED_20V_100W =
    PDO_FIXED_BASE | (PWR_V_50MV(20) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos) |
    (PWR_A_10MA(5) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos);

const uint32_t PDO_FIXED_5V_0W =
    PDO_FIXED_BASE | (PWR_V_50MV(5) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos) |
    (PWR_A_10MA(0) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos);
// NOLINTEND(misc-redundant-expression)

uint32_t PORT0_PDO_ListSNK[USBPD_MAX_NB_PDO] = {
    /* PDO 1 */
    PDO_FIXED_5V_0W,
    /* PDO 2 */
    PDO_FIXED_20V_45W,
    /* PDO 3 */
    PDO_FIXED_20V_100W,
    /* PDO 4 */
    (0x00000000U),
    /* PDO 5 */
    (0x00000000U),
    /* PDO 6 */
    (0x00000000U),
    /* PDO 7 */
    (0x00000000U)
};

USBPD_StatusTypeDef USBPD_PWR_IF_Init(void)
{
    return USBPD_ERROR;
}

USBPD_StatusTypeDef USBPD_PWR_IF_SupplyReady(
    uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe
)
{
    USBPD_StatusTypeDef status = USBPD_ERROR;
    uint32_t            _voltage;

    /* check for valid port */
    if (!USBPD_PORT_IsValid(PortNum)) {
        return USBPD_ERROR;
    }

    BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &_voltage);
    if (USBPD_VSAFE_0V == Vsafe) {
        /* Vsafe0V */
        status =
            ((_voltage < USBPD_PWR_LOW_VBUS_THRESHOLD) ? USBPD_OK : USBPD_ERROR
            );
    }
    else {
        /* Vsafe5V */
        status =
            ((_voltage > USBPD_PWR_HIGH_VBUS_THRESHOLD) ? USBPD_OK : USBPD_ERROR
            );
    }

    // TODO: remove next in future, hotfix for voltage control
    status = USBPD_OK;

    return status;
}

USBPD_StatusTypeDef USBPD_PWR_IF_ReadVA(
    uint8_t PortNum, uint16_t* pVoltage, uint16_t* pCurrent
)
{
    (void)PortNum;
    (void)pVoltage;
    (void)pCurrent;

    return USBPD_ERROR;
}

USBPD_StatusTypeDef USBPD_PWR_IF_Enable_VConn(
    uint8_t PortNum, CCxPin_TypeDef CCpin
)
{
    (void)PortNum;
    (void)CCpin;
    return USBPD_ERROR;
}

USBPD_StatusTypeDef USBPD_PWR_IF_Disable_VConn(
    uint8_t PortNum, CCxPin_TypeDef CCpin
)
{
    (void)PortNum;
    (void)CCpin;
    return USBPD_ERROR;
}

void USBPD_PWR_IF_GetPortPDOs(
    uint8_t                         PortNum,
    USBPD_CORE_DataInfoType_TypeDef DataId,
    uint8_t*                        Ptr,
    uint32_t*                       Size
)
{
    (void)PortNum;
    (void)DataId;
    {
        *Size = PORT0_NB_SINKPDO;
        memcpy(Ptr, PORT0_PDO_ListSNK, sizeof(uint32_t) * PORT0_NB_SINKPDO);
    }
}

USBPD_StatusTypeDef USBPD_PWR_IF_SearchRequestedPDO(
    uint8_t PortNum, uint32_t RdoPosition, uint32_t* Pdo
)
{
    (void)PortNum;
    (void)RdoPosition;
    (void)Pdo;

    return USBPD_FAIL;
}

void USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_ERROR ErrorType)
{
    (void)ErrorType;
}

void USBPD_PWR_IF_Alarm(void) {}

uint8_t USBPD_PWR_IF_GetVBUSStatus(
    uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus
)
{
    // TODO: Vbus control not used
    return USBPD_TRUE;

    uint8_t  _status = USBPD_FALSE;
    uint32_t _vbus;
    BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &_vbus);

    switch (PowerTypeStatus) {
        case USBPD_PWR_BELOWVSAFE0V:
            if (_vbus < USBPD_PWR_LOW_VBUS_THRESHOLD) {
                _status = USBPD_TRUE;
            }
            break;
        case USBPD_PWR_VSAFE5V:
            if (_vbus >= USBPD_PWR_HIGH_VBUS_THRESHOLD) {
                _status = USBPD_TRUE;
            }
            break;
        case USBPD_PWR_SNKDETACH:
            if (_vbus < USBPD_PWR_HIGH_VBUS_THRESHOLD) {
                _status = USBPD_TRUE;
            }
            break;
        default: break;
    }
    return _status;
}

void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum)
{
    (void)PortNum;
}

void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum)
{
    (void)PortNum;
}
