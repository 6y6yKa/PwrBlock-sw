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

#pragma once

#include <usbpd_def.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Request the PE to send a request message.
 * @param  PortNum     The current port number
 * @param  IndexSrcPDO Index on the selected SRC PDO (value between 1 to 7)
 * @param  RequestedVoltage Requested voltage (in MV and use mainly for APDO)
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestMessageRequest(
    uint8_t PortNum, uint8_t IndexSrcPDO, uint16_t RequestedVoltage
);

/**
 * @brief  Request the PE to send a hard reset
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestHardReset(uint8_t PortNum);

/**
  * @brief  Request the PE to send a cable reset.
  * @note   Only a DFP Shall generate Cable Reset Signaling. A DFP Shall only
  generate Cable Reset Signaling within an Explicit Contract. The DFP has to be
  supplying VCONN prior to a Cable Reset
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestCableReset(uint8_t PortNum);

/**
 * @brief  Request the PE to send a GOTOMIN message
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGotoMin(uint8_t PortNum);

/**
 * @brief  Request the PE to send a PING message
 * @note   In USB-PD stack, only ping management for P3.0 is implemented.
 *         If PD2.0 is used, PING timer needs to be implemented on user side.
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestPing(uint8_t PortNum);

/**
 * @brief  Request the PE to send a GET_SRC_CAPA message
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapability(uint8_t PortNum);

/**
 * @brief  Request the PE to send a GET_SNK_CAPA message
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapability(uint8_t PortNum);

/**
 * @brief  Request the PE to perform a Data Role Swap.
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestDataRoleSwap(uint8_t PortNum);

/**
 * @brief  Request the PE to perform a Power Role Swap.
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestPowerRoleSwap(uint8_t PortNum);

/**
 * @brief  Request the PE to perform a VCONN Swap.
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestVconnSwap(uint8_t PortNum);

/**
 * @brief  Request the PE to send a soft reset
 * @param  PortNum The current port number
 * @param  SOPType SOP Type based on @ref USBPD_SOPType_TypeDef
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestSoftReset(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType
);

/**
 * @brief  Request the PE to send a Source Capability message.
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestSourceCapability(uint8_t PortNum);

/**
 * @brief  Request the PE to send a VDM discovery identity
 * @param  PortNum The current port number
 * @param  SOPType SOP Type
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoveryIdentify(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType
);

/**
 * @brief  Request the PE to send a VDM discovery SVID
 * @param  PortNum The current port number
 * @param  SOPType SOP Type
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoverySVID(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType
);

/**
 * @brief  Request the PE to perform a VDM Discovery mode message on one SVID.
 * @param  PortNum The current port number
 * @param  SOPType SOP Type
 * @param  SVID    SVID used for discovery mode message
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoveryMode(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID
);

/**
 * @brief  Request the PE to perform a VDM mode enter.
 * @param  PortNum   The current port number
 * @param  SOPType   SOP Type
 * @param  SVID      SVID used for discovery mode message
 * @param  ModeIndex Index of the mode to be entered
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_EnterMode(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint8_t               ModeIndex
);

/**
 * @brief  Request the PE to perform a VDM mode exit.
 * @param  PortNum   The current port number
 * @param  SOPType   SOP Type
 * @param  SVID      SVID used for discovery mode message
 * @param  ModeIndex Index of the mode to be exit
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_ExitMode(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint8_t               ModeIndex
);

/**
 * @brief  Request the PE to send a Display Port status
 * @param  PortNum   The current port number
 * @param  SOPType   SOP Type
 * @param  SVID      Used SVID
 * @param  pDPStatus Pointer on DP Status data (32 bit)
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestDisplayPortStatus(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint32_t*             pDPStatus
);

/**
 * @brief  Request the PE to send a Display Port Config
 * @param  PortNum   The current port number
 * @param  SOPType   SOP Type
 * @param  SVID      Used SVID
 * @param  pDPConfig Pointer on DP Config data (32 bit)
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestDisplayPortConfig(
    uint8_t               PortNum,
    USBPD_SOPType_TypeDef SOPType,
    uint16_t              SVID,
    uint32_t*             pDPConfig
);

/**
 * @brief  Request the PE to perform a VDM Attention.
 * @param  PortNum The current port number
 * @param  SOPType SOP Type
 * @param  SVID    Used SVID
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestAttention(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID
);

/**
 * @brief  Request the PE to send an ALERT to port partner
 * @param  PortNum The current port number
 * @param  Alert   Alert based on @ref USBPD_ADO_TypeDef
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestAlert(
    uint8_t PortNum, USBPD_ADO_TypeDef Alert
);

/**
 * @brief  Request the PE to get a source capability extended
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapabilityExt(uint8_t PortNum);

/**
 * @brief  Request the PE to get a sink capability extended
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapabilityExt(uint8_t PortNum);

/**
 * @brief  Request the PE to get a manufacturer info
 * @param  PortNum The current port number
 * @param  SOPType SOP Type
 * @param  pManuInfoData Pointer on manufacturer info based on @ref
 * USBPD_GMIDB_TypeDef
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetManufacturerInfo(
    uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint8_t* pManuInfoData
);

/**
 * @brief  Request the PE to request a GET_STATUS
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetStatus(uint8_t PortNum);

/**
 * @brief  Request the PE to perform a Fast Role Swap.
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestFastRoleSwap(uint8_t PortNum);

/**
 * @brief  Request the PE to request a GET_PPS_STATUS
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetPPS_Status(uint8_t PortNum);

/**
 * @brief  Request the PE to send a GET_COUNTRY_CODES message
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryCodes(uint8_t PortNum);

/**
 * @brief  Request the PE to send a GET_COUNTRY_INFO message
 * @param  PortNum     The current port number
 * @param  CountryCode Country code (1st character and 2nd of the Alpha-2
 * Country)
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryInfo(
    uint8_t PortNum, uint16_t CountryCode
);

/**
 * @brief  Request the PE to send a GET_BATTERY_CAPA
 * @param  PortNum         The current port number
 * @param  pBatteryCapRef  Pointer on the Battery Capability reference
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryCapability(
    uint8_t PortNum, uint8_t* pBatteryCapRef
);

/**
 * @brief  Request the PE to send a GET_BATTERY_STATUS
 * @param  PortNum           The current port number
 * @param  pBatteryStatusRef Pointer on the Battery Status reference
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryStatus(
    uint8_t PortNum, uint8_t* pBatteryStatusRef
);

/**
 * @brief  Request the PE to send a SECURITY_REQUEST
 * @param  PortNum The current port number
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_RequestSecurityRequest(uint8_t PortNum);


#ifdef __cplusplus
}
#endif
