/*
 * Copyright 2026 Everypin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dpm_usbpd_if.h"

#include <usbpd_def.h>

#include "dpm_trace_def.h"

USBPD_StatusTypeDef USBPD_DPM_RequestMessageRequest(
    uint8_t PortNum, uint8_t IndexSrcPDO, uint16_t RequestedVoltage
)
{
    (void)PortNum;
    (void)IndexSrcPDO;
    (void)RequestedVoltage;
    USBPD_StatusTypeDef _status = USBPD_ERROR;

    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestHardReset(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_HardReset(PortNum);
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "HARD RESET not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestCableReset(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CableReset(PortNum);
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "CABLE RESET not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGotoMin(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GOTOMIN, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(PortNum, _status, "GOTOMIN not accepted by the stack");
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestPing(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_PING, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(PortNum, _status, "PING not accepted by the stack");
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapability(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_SRC_CAP, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_SRC_CAPA not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapability(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_SNK_CAP, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_SINK_CAPA not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestDataRoleSwap(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_DR_SWAP, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(PortNum, _status, "DRS not accepted by the stack");
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestPowerRoleSwap(uint8_t PortNum)
{
    DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "PRS not accepted by the stack");
    return USBPD_ERROR;
}

USBPD_StatusTypeDef USBPD_DPM_RequestVconnSwap(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_VCONN_SWAP, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(PortNum, _status, "VCS not accepted by the stack");
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestSoftReset(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType
)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_SOFT_RESET, SOPType
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "SOFT_RESET not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestSourceCapability(uint8_t PortNum)
{
    /* PE will directly get the PDO saved in structure @ref PWR_Port_PDO_Storage
     */
    USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(
        PortNum, USBPD_DATAMSG_SRC_CAPABILITIES, NULL
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "SRC_CAPA not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoveryIdentify(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType
)
{
    (void)PortNum;
    (void)SOPType;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "VDM Discovery Ident not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoverySVID(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType
)
{
    (void)PortNum;
    (void)SOPType;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "VDM discovery SVID not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoveryMode(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID
)
{
    (void)PortNum;
    (void)SOPType;
    (void)SVID;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "VDM Discovery mode not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestVDM_EnterMode(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint8_t               ModeIndex
)
{
    (void)PortNum;
    (void)SOPType;
    (void)SVID;
    (void)ModeIndex;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "VDM mode enter not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestVDM_ExitMode(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint8_t               ModeIndex
)
{
    (void)PortNum;
    (void)SOPType;
    (void)SVID;
    (void)ModeIndex;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "VDM mode exit not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestDisplayPortStatus(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint32_t*             pDPStatus
)
{
    (void)PortNum;
    (void)SOPType;
    (void)SVID;
    (void)pDPStatus;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "Display Port status not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestDisplayPortConfig(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint32_t*             pDPConfig
)
{
    (void)PortNum;
    (void)SOPType;
    (void)SVID;
    (void)pDPConfig;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "Display Port Config not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestAttention(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID
)
{
    (void)PortNum;
    (void)SOPType;
    (void)SVID;

    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "VDM ATTENTION not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestAlert(
    uint8_t PortNum, USBPD_ADO_TypeDef Alert
)
{
    USBPD_StatusTypeDef _status =
        USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_ALERT, &Alert.d32);
    DPM_USER_ERROR_TRACE(PortNum, _status, "ALERT not accepted by the stack");
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapabilityExt(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_SRC_CAPEXT, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_SRC_CAPA_EXT not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapabilityExt(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_SNK_CAPEXT, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_SINK_CAPA_EXT not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetManufacturerInfo(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint8_t* pManuInfoData
)
{
    USBPD_StatusTypeDef _status = USBPD_ERROR;
    if (USBPD_SOPTYPE_SOP == SOPType) {
        _status = USBPD_PE_SendExtendedMessage(
            PortNum,
            SOPType,
            USBPD_EXT_GET_MANUFACTURER_INFO,
            pManuInfoData,
            sizeof(USBPD_GMIDB_TypeDef)
        );
    }
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_MANU_INFO not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetPPS_Status(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_PPS_STATUS, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_PPS_STATUS not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetStatus(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_STATUS, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_STATUS not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestFastRoleSwap(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_FR_SWAP, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(PortNum, _status, "FRS not accepted by the stack");
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryCodes(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(
        PortNum, USBPD_CONTROLMSG_GET_COUNTRY_CODES, USBPD_SOPTYPE_SOP
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_COUNTRY_CODES not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryInfo(
    uint8_t PortNum, uint16_t CountryCode
)
{
    USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(
        PortNum, USBPD_DATAMSG_GET_COUNTRY_INFO, (uint32_t*)&CountryCode
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_COUNTRY_INFO not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryCapability(
    uint8_t PortNum, uint8_t* pBatteryCapRef
)
{
    USBPD_StatusTypeDef _status = USBPD_PE_SendExtendedMessage(
        PortNum, USBPD_SOPTYPE_SOP, USBPD_EXT_GET_BATTERY_CAP, pBatteryCapRef, 1
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_BATTERY_CAPA not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryStatus(
    uint8_t PortNum, uint8_t* pBatteryStatusRef
)
{
    USBPD_StatusTypeDef _status = USBPD_PE_SendExtendedMessage(
        PortNum,
        USBPD_SOPTYPE_SOP,
        USBPD_EXT_GET_BATTERY_STATUS,
        pBatteryStatusRef,
        1
    );
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "GET_BATTERY_STATUS not accepted by the stack"
    );
    return _status;
}

USBPD_StatusTypeDef USBPD_DPM_RequestSecurityRequest(uint8_t PortNum)
{
    USBPD_StatusTypeDef _status = USBPD_ERROR;
    DPM_USER_ERROR_TRACE(
        PortNum, _status, "SECURITY_REQUEST not accepted by the stack"
    );
    return _status;
}
