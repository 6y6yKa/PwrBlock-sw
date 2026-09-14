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
 * @brief  Initialize DPM (port power role, PWR_IF, CAD and PE Init procedures)
 * @retval USBPD Status
 */
USBPD_StatusTypeDef USBPD_DPM_UserInit(void);

/**
 * @brief  CableDetection reporting events on a specified port from CAD
 * layer.
 * @param  PortNum The handle of the port
 * @param  State CAD state
 * @retval None
 */
void USBPD_DPM_CableDetection(uint8_t PortNum, USBPD_CAD_EVENT State);

/**
 * @brief  Callback function called by PE to inform DPM about PE event.
 * @param  PortNum The current port number
 * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
 * @retval None
 */
void USBPD_DPM_Notification(
    uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal
);

/**
 * @brief  Callback function called by PE layer when HardReset message received
 * from PRL
 * @param  PortNum The current port number
 * @param  CurrentRole the current role
 * @param  Status status on hard reset event
 * @retval None
 */
void USBPD_DPM_HardReset(
    uint8_t                     PortNum,
    USBPD_PortPowerRole_TypeDef CurrentRole,
    USBPD_HR_Status_TypeDef     Status
);

/**
 * @brief  DPM callback to allow PE to forward extended message information.
 * @param  PortNum Port number
 * @param  MsgType Type of message to be handled in DPM
 *         This parameter can be one of the following values:
 *           @arg @ref USBPD_EXT_SECURITY_REQUEST Security Request extended
 * message
 *           @arg @ref USBPD_EXT_SECURITY_RESPONSE Security Response extended
 * message
 * @param  ptrData   Pointer on address Extended Message data could be read (u8
 * pointer)
 * @param  DataSize  Nb of u8 that compose Extended message
 * @retval None
 */
void USBPD_DPM_ExtendedMessageReceived(
    uint8_t                   PortNum,
    USBPD_ExtendedMsg_TypeDef MsgType,
    uint8_t*                  ptrData,
    uint16_t                  DataSize
);

/**
 * @brief  DPM callback to allow PE to retrieve information from DPM/PWR_IF.
 * @param  PortNum Port number
 * @param  DataId  Type of data to be updated in DPM based on @ref
 * USBPD_CORE_DataInfoType_TypeDef
 * @param  Ptr     Pointer on address where DPM data should be written (u8
 * pointer)
 * @param  Size    Pointer on nb of u8 written by DPM
 * @retval None
 */
void USBPD_DPM_GetDataInfo(
    uint8_t                         PortNum,
    USBPD_CORE_DataInfoType_TypeDef DataId,
    uint8_t*                        Ptr,
    uint32_t*                       Size
);

/**
 * @brief  DPM callback to allow PE to update information in DPM/PWR_IF.
 * @param  PortNum Port number
 * @param  DataId  Type of data to be updated in DPM based on @ref
 * USBPD_CORE_DataInfoType_TypeDef
 * @param  Ptr     Pointer on the data
 * @param  Size    Nb of bytes to be updated in DPM
 * @retval None
 */
void USBPD_DPM_SetDataInfo(
    uint8_t                         PortNum,
    USBPD_CORE_DataInfoType_TypeDef DataId,
    uint8_t*                        Ptr,
    uint32_t                        Size
);

/**
 * @brief  Evaluate received Capabilities Message from Source port and prepare
 * the request message
 * @param  PortNum         Port number
 * @param  PtrRequestData  Pointer on selected request data object
 * @param  PtrPowerObjectType  Pointer on the power data object
 * @retval None
 */
void USBPD_DPM_SNK_EvaluateCapabilities(
    uint8_t                      PortNum,
    uint32_t*                    PtrRequestData,
    USBPD_CORE_PDO_Type_TypeDef* PtrPowerObjectType
);

/**
 * @brief  Callback to be used by PE to evaluate a Vconn swap
 * @param  PortNum Port number
 * @retval USBPD_ACCEPT, USBPD_REJECT, USBPD_WAIT
 */
USBPD_StatusTypeDef USBPD_DPM_EvaluateVconnSwap(uint8_t PortNum);

/**
 * @brief  Callback to be used by PE to manage VConn
 * @param  PortNum Port number
 * @param  State Enable or Disable VConn on CC lines
 * @retval USBPD_ACCEPT, USBPD_REJECT
 */
USBPD_StatusTypeDef USBPD_DPM_PE_VconnPwr(
    uint8_t PortNum, USBPD_FunctionalState State
);

/**
 * @brief  DPM callback to allow PE to enter ERROR_RECOVERY state.
 * @param  PortNum Port number
 * @retval None
 */
void USBPD_DPM_EnterErrorRecovery(uint8_t PortNum);

/**
  * @brief  Callback used to ask application the reply status for a DataRoleSwap
  request
  * @note   if the callback is not set (ie NULL) the stack will automatically
  reject the request
  * @param  PortNum Port number
  * @retval Returned values are:
            @ref USBPD_ACCEPT if DRS can be accepted
            @ref USBPD_REJECT if DRS is not accepted in one data role (DFP or
  UFP) or in PD2.0 config
            @ref USBPD_NOTSUPPORTED if DRS is not supported at all by the
  application (in both data roles) - P3.0 only
  */
USBPD_StatusTypeDef USBPD_DPM_EvaluateDataRoleSwap(uint8_t PortNum);

/**
 * @brief  Callback to be used by PE to check is VBUS is ready or present
 * @param  PortNum Port number
 * @param  Vsafe   Vsafe status based on @ref USBPD_VSAFE_StatusTypeDef
 * @retval USBPD_DISABLE or USBPD_ENABLE
 */
USBPD_FunctionalState USBPD_DPM_IsPowerReady(
    uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe
);

#ifdef __cplusplus
}
#endif
