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

#include "dpm.h"

#include <stdbool.h>
#include <usbpd_core.h>

#include "dpm_conf.h"
#include "dpm_def.h"
#include "dpm_trace_def.h"
#include "usbpd_dpm_core.h"
#include "usbpd_pwr_if.h"

#include "regulator/regulator_task.h"
#include "sys_status/sys_status.h"

/** @brief  Sink Request characteritics Structure definition
 *
 */
typedef struct
{
    //! Sink request operating voltage in mV units
    uint32_t RequestedVoltageInmVunits;
    //! Sink request Max operating current in mA units
    uint32_t MaxOperatingCurrentInmAunits;
    //! Sink request operating current in mA units
    uint32_t OperatingCurrentInmAunits;
    //! Sink request Max operating power in mW units
    uint32_t MaxOperatingPowerInmWunits;
    //! Sink request operating power in mW units
    uint32_t OperatingPowerInmWunits;
} USBPD_DPM_SNKPowerRequestDetails_TypeDef;

/* Method used to find the "best" PDO */
USBPD_DPM_PDO_SelectionMethodTypeDef USBPD_DPM_PDO_Sel_Method =
    PDO_SEL_METHOD_MAX_PWR;

// Handler array for handle multiple UCPD port
USBPD_HandleTypeDef DPM_Ports[USBPD_PORT_COUNT];

typedef struct
{
    uint32_t voltage;
    uint32_t current;
    uint32_t power;
} PowerProfile;

/**
 * @brief  Examinate a given SRC PDO to check if matching with SNK
 * capabilities.
 * @param  PortNum             Port number
 * @param  SrcPDO              Selected SRC PDO (32 bits)
 * @param  Request pointer to power parameter structure
 * could be requested from SRC. Values in millis.
 * Valid when return value is USBPD_TRUE.
 * @retval USBPD_FALSE of USBPD_TRUE (USBPD_TRUE returned in SRC PDO is
 * considered matching with SNK profile)
 */
static uint32_t DPM_SNK_EvaluateMatchWithSRCPDO(
    uint8_t PortNum, uint32_t SrcPDO, PowerProfile* Request
);

/**
 * @brief  Build RDO to be used in Request message according to selected PDO
 * from received SRC Capabilities
 * @param  PortNum           Port number
 * @param  IndexSrcPDO       Index on the selected SRC PDO (value between 0 to
 * 6)
 * @param  PtrRequestPowerDetails  Sink requested power details structure
 * pointer
 * @param  Rdo               Pointer on the RDO
 * @param  PtrPowerObject    Pointer on the selected power object
 * @retval None
 */
static void DPM_SNK_BuildRDOfromSelectedPDO(
    uint8_t                                   PortNum,
    uint8_t                                   IndexSrcPDO,
    USBPD_DPM_SNKPowerRequestDetails_TypeDef* PtrRequestPowerDetails,
    USBPD_SNKRDO_TypeDef*                     Rdo,
    USBPD_CORE_PDO_Type_TypeDef*              PtrPowerObject
);

/**
 * @brief  Find PDO index that offers the most amount of power and in accordance
 * with SNK capabilities.
 * @param  PortNum Port number
 * @param  PtrRequestPowerDetails  Sink requested power details structure
 * pointer
 * @retval Index of PDO within source capabilities message (DPM_NO_SRC_PDO_FOUND
 * indicating not found)
 */
static uint32_t DPM_FindVoltageIndex(
    uint32_t                                  PortNum,
    USBPD_DPM_SNKPowerRequestDetails_TypeDef* PtrRequestPowerDetails,
    USBPD_DPM_PDO_SelectionMethodTypeDef      Method
);

USBPD_StatusTypeDef USBPD_DPM_UserInit(void)
{
    return USBPD_OK;
}

void USBPD_DPM_CableDetection(uint8_t PortNum, USBPD_CAD_EVENT State)
{
    (void)State;
    DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: update USBPD_DPM_CableDetection");
}

void USBPD_DPM_Notification(
    uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal
)
{
    /* Manage event notified by the stack? */
    switch (EventVal) {
        case USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT: {
            sys_status_set(SYS_STATUS_SRC_PWR_IN, SYS_STATUS_OK);
            regulator_init();
            break;
        }
        case USBPD_NOTIFY_USBSTACK_STOP:
        case USBPD_NOTIFY_HARDRESET_RX:
        case USBPD_NOTIFY_HARDRESET_TX:
        case USBPD_NOTIFY_DETACH: {
            sys_status_set(SYS_STATUS_SRC_PWR_IN, SYS_STATUS_OFF);
            // Driver do not recognize commands at this point
            regulator_deinit();
            break;
        }
        case USBPD_NOTIFY_PE_DISABLED:
        case USBPD_NOTIFY_POWER_SNK_STOP: {
            sys_status_set(SYS_STATUS_SRC_PWR_IN, SYS_STATUS_FAULT);
            break;
        }
        default:
            DPM_USER_DEBUG_TRACE(PortNum, "notif code: %d", EventVal);
            break;
    }
}

void USBPD_DPM_HardReset(
    uint8_t                     PortNum,
    USBPD_PortPowerRole_TypeDef CurrentRole,
    USBPD_HR_Status_TypeDef     Status
)
{
    (void)CurrentRole;
    (void)Status;

    DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: update USBPD_DPM_HardReset");
}

void USBPD_DPM_GetDataInfo(
    uint8_t                         PortNum,
    USBPD_CORE_DataInfoType_TypeDef DataId,
    uint8_t*                        Ptr,
    uint32_t*                       Size
)
{
    // Check type of information targeted by request
    switch (DataId) {
        //! Handling of port Sink PDO, requested by get sink capa
        case USBPD_CORE_DATATYPE_SNK_PDO:
            USBPD_PWR_IF_GetPortPDOs(PortNum, DataId, Ptr, Size);
            *Size *= 4;
            break;
        //! Get voltage value requested for BIST tests, expect 5V
        case USBPD_CORE_DATATYPE_REQ_VOLTAGE:
            *Size = 4;
            (void)memcpy(
                Ptr, (uint8_t*)&DPM_Ports[PortNum].DPM_RequestedVoltage, *Size
            );
            break;
        case USBPD_CORE_REVISION: {
            *Size = sizeof(USBPD_RevisionDO_TypeDef);
            //! Hardcoded values, user should use a global
            //! USBPD_RevisionDO_TypeDef variable
            USBPD_RevisionDO_TypeDef rev = {
                //! Major revision
                .b.Revision_major = USBPD_REV_MAJOR,
                //! Minor revision
                .b.Revision_minor = USBPD_REV_MINOR,
                //! Major version
                .b.Version_major = USBPD_VERSION_MAJOR,
                //! Minor version
                .b.Version_minor = USBPD_VERSION_MINOR
            };

            memcpy(Ptr, &rev, *Size);
            break;
        }
#if defined(USBPDCORE_SNK_CAPA_EXT)
        case USBPD_CORE_SNK_EXTENDED_CAPA: {
            *Size = sizeof(USBPD_SKEDB_TypeDef);
            memcpy(
                Ptr,
                (uint8_t*)&DPM_USER_Settings[PortNum].DPM_SNKExtendedCapa,
                *Size
            );
        } break;
#endif /* USBPDCORE_SNK_CAPA_EXT */
        default: break;
    }
}

void USBPD_DPM_SetDataInfo(
    uint8_t                         PortNum,
    USBPD_CORE_DataInfoType_TypeDef DataId,
    uint8_t*                        Ptr,
    uint32_t                        Size
)
{
    /* Check type of information targeted by request */
    switch (DataId) {
        case USBPD_CORE_DATATYPE_RCV_REQ_PDO: /*!< Storage of Received Sink
                                                 Request PDO value */
            if (Size == 4) {
                memcpy(
                    (uint8_t*)&DPM_Ports[PortNum].DPM_RcvRequestDOMsg, Ptr, 4
                );
            }
            break;

            /* Case Received Source PDO values Data information : */
        case USBPD_CORE_DATATYPE_RCV_SRC_PDO:
            if (Size <= (USBPD_MAX_NB_PDO * 4)) {
                uint8_t* rdo;
                DPM_Ports[PortNum].DPM_NumberOfRcvSRCPDO = (Size / 4);
                /* Copy PDO data in DPM Handle field */
                for (uint32_t index = 0; index < (Size / 4); index++) {
                    rdo = (uint8_t*)&DPM_Ports[PortNum]
                              .DPM_ListOfRcvSRCPDO[index];
                    (void
                    )memcpy(rdo, (Ptr + (index * 4U)), (4U * sizeof(uint8_t)));
                }
            }
            break;

        case USBPD_CORE_REVISION: {
            /* Does nothing: User have to implement a global revision variable
             */
            USBPD_RevisionDO_TypeDef rev = {0};
            memcpy((uint8_t*)&rev, Ptr, sizeof(USBPD_RevisionDO_TypeDef));
            break;
        }

#if defined(USBPDCORE_SNK_CAPA_EXT)
        case USBPD_CORE_SNK_EXTENDED_CAPA: {
            uint8_t* _snk_ext_capa;
            _snk_ext_capa =
                (uint8_t*)&DPM_Ports[PortNum].DPM_RcvSNKExtendedCapa;
            memcpy(_snk_ext_capa, Ptr, Size);
            break;
        }
#endif /* USBPDCORE_SNK_CAPA_EXT */

        default:
            DPM_USER_DEBUG_TRACE(
                PortNum, "ADVICE: update USBPD_DPM_SetDataInfo:%d", DataId
            );
            break;
    }
}

void USBPD_DPM_SNK_EvaluateCapabilities(
    uint8_t                      PortNum,
    uint32_t*                    PtrRequestData,
    USBPD_CORE_PDO_Type_TypeDef* PtrPowerObjectType
)
{
    USBPD_SNKRDO_TypeDef                     rdo     = {0};
    USBPD_DPM_SNKPowerRequestDetails_TypeDef pwr_req = {0, 0, 0, 0, 0};
    uint32_t                                 srcpdo_idx;

    srcpdo_idx =
        DPM_FindVoltageIndex(PortNum, &pwr_req, USBPD_DPM_PDO_Sel_Method);

    DPM_SNK_BuildRDOfromSelectedPDO(
        PortNum, srcpdo_idx, &pwr_req, &rdo, PtrPowerObjectType
    );

    *PtrRequestData = rdo.d32;
}

USBPD_StatusTypeDef USBPD_DPM_EvaluateVconnSwap(uint8_t PortNum)
{
    USBPD_StatusTypeDef status = USBPD_REJECT;
    if (USBPD_TRUE == DPM_USER_Settings[PortNum].PE_VconnSwap) {
        status = USBPD_ACCEPT;
    }

    return status;
}

USBPD_StatusTypeDef USBPD_DPM_PE_VconnPwr(
    uint8_t PortNum, USBPD_FunctionalState State
)
{
    (void)PortNum;
    (void)State;
    return USBPD_ERROR;
}

void USBPD_DPM_ExtendedMessageReceived(
    uint8_t                   PortNum,
    USBPD_ExtendedMsg_TypeDef MsgType,
    uint8_t*                  ptrData,
    uint16_t                  DataSize
)
{
    (void)PortNum;
    (void)MsgType;
    (void)ptrData;
    (void)DataSize;
}

void USBPD_DPM_EnterErrorRecovery(uint8_t PortNum)
{
    // Inform CAD to enter recovery mode
    USBPD_CAD_EnterErrorRecovery(PortNum);
}

USBPD_StatusTypeDef USBPD_DPM_EvaluateDataRoleSwap(uint8_t PortNum)
{
    USBPD_StatusTypeDef status = USBPD_REJECT;
    /* Sent NOT_SUPPORTED if DRS is not supported at all by the application (in
     * both data roles) - P3.0 only */
    if ((USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DataSwap) ||
        ((USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_DFP) &&
         (USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_UFP))) {
        status = USBPD_NOTSUPPORTED;
    }
    else {
        /* ACCEPT DRS if at least supported by 1 data role */
        if (((USBPD_TRUE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_DFP) &&
             (USBPD_PORTDATAROLE_UFP == DPM_Params[PortNum].PE_DataRole)) ||
            ((USBPD_TRUE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_UFP) &&
             (USBPD_PORTDATAROLE_DFP == DPM_Params[PortNum].PE_DataRole))) {
            status = USBPD_ACCEPT;
        }
    }
    return status;
}

USBPD_FunctionalState USBPD_DPM_IsPowerReady(
    uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe
)
{
    return (
        (USBPD_OK == USBPD_PWR_IF_SupplyReady(PortNum, Vsafe)) ? USBPD_ENABLE :
                                                                 USBPD_DISABLE
    );
}

uint32_t DPM_SNK_EvaluateMatchWithSRCPDO(
    uint8_t PortNum, uint32_t SrcPDO, PowerProfile* request
)
{
    USBPD_PDO_TypeDef srcpdo;
    srcpdo.d32 = SrcPDO;

    if (srcpdo.GenericPDO.PowerObject != USBPD_CORE_PDO_TYPE_FIXED) {
        // SRC PDO type not supported
        return USBPD_FALSE;
    }

    PowerProfile src;
    src.voltage = srcpdo.SRCFixedPDO.VoltageIn50mVunits;
    src.current = srcpdo.SRCFixedPDO.MaxCurrentIn10mAunits;

    // Retrieve SNK PDO list from PWR_IF storage
    uint32_t snkpdo_num;
    uint32_t snkpdo_array[USBPD_MAX_NB_PDO];
    USBPD_PWR_IF_GetPortPDOs(
        PortNum,
        USBPD_CORE_DATATYPE_SNK_PDO,
        (uint8_t*)snkpdo_array,
        &snkpdo_num
    );

    if (0 == snkpdo_num) {
        // No SNK PDO Profiles
        return (USBPD_FALSE);
    }

    PowerProfile req_max = {0, 0, 0};
    uint32_t     match   = USBPD_FALSE;
    for (uint32_t i = 0; i < snkpdo_num; i++) {
        USBPD_PDO_TypeDef snkpdo;
        /* Retrieve SNK PDO value according to its type */
        snkpdo.d32 = snkpdo_array[i];
        if (snkpdo.GenericPDO.PowerObject != USBPD_CORE_PDO_TYPE_FIXED) {
            // SNK PDO type not supported
            continue;
        }

        PowerProfile snk;
        snk.voltage = snkpdo.SNKFixedPDO.VoltageIn50mVunits;
        snk.current = snkpdo.SNKFixedPDO.OperationalCurrentIn10mAunits;

        PowerProfile req_temp = {0, 0, 0};
        if ((snk.voltage == src.voltage) && (snk.current <= src.current)) {
            req_temp.voltage = src.voltage;
            req_temp.current = src.current;
            req_temp.power   = src.voltage * src.current;
        }

        if (req_temp.power > req_max.power) {
            match           = USBPD_TRUE;
            req_max.voltage = req_temp.voltage;
            req_max.current = req_temp.current;
            req_max.power   = req_temp.power;
        }
    }

    if (req_max.power > 0) {
        request->voltage = req_max.voltage * 50;  // to mV
        request->current = req_max.current * 10;  // to mA
        request->power   = req_max.power / 2;     // to mW
    }
    return (match);
}

static uint32_t DPM_FindVoltageIndex(
    uint32_t                                  PortNum,
    USBPD_DPM_SNKPowerRequestDetails_TypeDef* PtrRequestPowerDetails,
    USBPD_DPM_PDO_SelectionMethodTypeDef      Method
)
{
    // Search PDO index among Source PDO of Port
    uint32_t* srcpdo_array_ptr = DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO;
    uint32_t  srcpdo_num       = DPM_Ports[PortNum].DPM_NumberOfRcvSRCPDO;

    uint32_t     srcpdo_sel_idx = DPM_NO_SRC_PDO_FOUND;
    PowerProfile selected       = {0, 0, 0};
    // Search the best PDO in the list of source PDOs
    for (uint32_t srcpdo_idx = 0; srcpdo_idx < srcpdo_num; srcpdo_idx++) {
        USBPD_PDO_TypeDef srcpdo;
        srcpdo.d32 = srcpdo_array_ptr[srcpdo_idx];

        PowerProfile allowed = {0, 0, 0};
        // Check if the received source PDO is matching any of the SNK PDO
        uint32_t match =
            DPM_SNK_EvaluateMatchWithSRCPDO(PortNum, srcpdo.d32, &allowed);

        if (match == USBPD_TRUE) {
            // Choose the best PDO depending on the user preferences
            switch (Method) {
                case PDO_SEL_METHOD_MAX_PWR:
                    if (allowed.power > selected.power) {
                        // Consider the current PDO the best one until now
                        srcpdo_sel_idx   = srcpdo_idx;
                        selected.power   = allowed.power;
                        selected.voltage = allowed.voltage;
                        selected.current = allowed.current;
                    }
                    break;
                default:
                    // Default behavior: last PDO is selected
                    srcpdo_sel_idx   = srcpdo_idx;
                    selected.power   = allowed.power;
                    selected.voltage = allowed.voltage;
                    selected.current = allowed.current;
            }
        }
    }

    // If a suitable PDO was found
    USBPD_USER_SettingsTypeDef* puser = &DPM_USER_Settings[PortNum];
    if (srcpdo_sel_idx != DPM_NO_SRC_PDO_FOUND) {
        // Fill the request power details
        PtrRequestPowerDetails->MaxOperatingCurrentInmAunits =
            puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits;
        PtrRequestPowerDetails->OperatingCurrentInmAunits = selected.current;
        PtrRequestPowerDetails->MaxOperatingPowerInmWunits =
            puser->DPM_SNKRequestedPower.MaxOperatingPowerInmWunits;
        PtrRequestPowerDetails->OperatingPowerInmWunits   = selected.power;
        PtrRequestPowerDetails->RequestedVoltageInmVunits = selected.voltage;
    }

    return srcpdo_sel_idx;
}

void DPM_SNK_BuildRDOfromSelectedPDO(
    uint8_t                                   PortNum,
    uint8_t                                   IndexSrcPDO,
    USBPD_DPM_SNKPowerRequestDetails_TypeDef* PtrRequestPowerDetails,
    USBPD_SNKRDO_TypeDef*                     Rdo,
    USBPD_CORE_PDO_Type_TypeDef*              PtrPowerObject
)
{
    bool                        default_rdo = false;
    uint32_t                    snkpdo_num;
    USBPD_HandleTypeDef*        pdhandle = &DPM_Ports[PortNum];
    USBPD_USER_SettingsTypeDef* puser    = &DPM_USER_Settings[PortNum];
    uint32_t                    snkpdo_array[USBPD_MAX_NB_PDO];

    /* Read SNK PDO list for retrieving useful data to fill in RDO */
    USBPD_PWR_IF_GetPortPDOs(
        PortNum,
        USBPD_CORE_DATATYPE_SNK_PDO,
        (uint8_t*)&snkpdo_array[0],
        &snkpdo_num
    );

    /* Store value of 1st SNK PDO (Fixed) in local variable */
    USBPD_SNKRDO_TypeDef rdo            = {0};
    USBPD_PDO_TypeDef    snkpdo_default = {snkpdo_array[0]};
    USBPD_PDO_TypeDef    srcpdo_default = {pdhandle->DPM_ListOfRcvSRCPDO[0]};

    /* Set common fields in RDO */
    rdo.GenericRDO.USBCommunicationsCapable =
        snkpdo_default.SNKFixedPDO.USBCommunicationsCapable;
    if (DPM_Params[PortNum].PE_SpecRevision > USBPD_SPECIFICATION_REV2) {
        rdo.FixedVariableRDO.UnchunkedExtendedMessage =
            DPM_Settings[PortNum].PE_PD3_Support.d.PE_UnchunkSupport;
        DPM_Params[PortNum].PE_UnchunkSupport = USBPD_FALSE;

        /* Set unchuncked bit if supported by port partner;*/
        if (srcpdo_default.SRCFixedPDO.UnchunkedExtendedMessage == USBPD_TRUE) {
            DPM_Params[PortNum].PE_UnchunkSupport = USBPD_TRUE;
        }
    }

    // If no valid SNK PDO or if no SRC PDO match found
    if ((snkpdo_num < 1) || (IndexSrcPDO >= pdhandle->DPM_NumberOfRcvSRCPDO)) {
        default_rdo = true;
    }

    USBPD_PDO_TypeDef srcpdo = {pdhandle->DPM_ListOfRcvSRCPDO[IndexSrcPDO]};
    // Check SRC PDO is fixed.
    if (srcpdo.GenericPDO.PowerObject != USBPD_CORE_PDO_TYPE_FIXED) {
        default_rdo = true;
    }

    PowerProfile request;
    if (default_rdo) {
        *PtrPowerObject = srcpdo_default.GenericPDO.PowerObject;

        request.voltage = srcpdo_default.SRCFixedPDO.VoltageIn50mVunits;
        request.current = srcpdo_default.SRCFixedPDO.MaxCurrentIn10mAunits;

        rdo.FixedVariableRDO.ObjectPosition               = 1;
        rdo.FixedVariableRDO.CapabilityMismatch           = 1;
        rdo.FixedVariableRDO.OperatingCurrentIn10mAunits  = request.current;
        rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits = request.current;

#ifdef _TRACE
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t *)"DPM_SNK_BuildRDOfromSelectedPDO: Pb in SRC PDO selection",
                    sizeof("DPM_SNK_BuildRDOfromSelectedPDO: Pb in SRC PDO selection"));
#endif /* _TRACE */
    }
    else {
        /* Extract power information from Power Data Object */
        *PtrPowerObject = srcpdo.GenericPDO.PowerObject;

        /* Retrieve request details from SRC PDO selection */
        request.voltage = PtrRequestPowerDetails->RequestedVoltageInmVunits;
        request.current = PtrRequestPowerDetails->OperatingCurrentInmAunits;
        request.power   = PtrRequestPowerDetails->OperatingPowerInmWunits;

        uint32_t src_max_current =
            PtrRequestPowerDetails->OperatingCurrentInmAunits;

        uint32_t snk_max_current =
            puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits;
        uint32_t snk_max_power =
            PtrRequestPowerDetails->MaxOperatingPowerInmWunits;

        // Limit current on SRC port
        if (src_max_current > snk_max_current) {
            request.current = snk_max_current;
            request.power   = snk_max_power;
        }

        // Build RDO
        rdo.FixedVariableRDO.ObjectPosition = IndexSrcPDO + 1;
        rdo.FixedVariableRDO.NoUSBSuspend   = 0;
        if (request.power < snk_max_power) {
            // TODO: Need API to check capability mismatch from App
            rdo.FixedVariableRDO.CapabilityMismatch = 1;
        }
        rdo.FixedVariableRDO.OperatingCurrentIn10mAunits = request.current / 10;
        rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits =
            snk_max_current / 10;
    }

    // Save RDO state to DPM port handler
    pdhandle->DPM_RequestDOMsg     = rdo.d32;
    pdhandle->DPM_RDOPosition      = rdo.GenericRDO.ObjectPosition;
    pdhandle->DPM_RequestedCurrent = request.current;
    pdhandle->DPM_RequestedVoltage = request.voltage;

    // Return builded RDO
    Rdo->d32 = rdo.d32;
}
