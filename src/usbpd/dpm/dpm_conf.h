/**
 ******************************************************************************
 * @file    dpm_conf.h
 * @author  MCD Application Team
 * @brief   Header file for DPM configuration
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
 * Projects/NUCLEO-G071RB/Applications/USB_PD/SNK1M1_Sink/Inc/usbpd_dpm_conf.h
 * (renamed usbpd_dpm_conf.h -> dpm_conf.h)
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "dpm_def.h"

/* Define   ------------------------------------------------------------------*/
/* Define VID, PID,... manufacturer parameters */
// TODO: Move to common USB config or use USB TMC descriptors
#define USBPD_VID                                                              \
    (0x0483u) /*!< Vendor ID (assigned by the USB-IF)                     */
#define USBPD_PID                                                              \
    (0x0002u) /*!< Product ID (assigned by the manufacturer)              */
#define USBPD_XID                                                              \
    (0xF0000003u) /*!< Value provided by the USB-IF assigned to the product */

/* Public variables ---------------------------------------------------------*/

#ifndef USBPD_DPM_CORE_C  // include in others files
extern USBPD_SettingsTypeDef      DPM_Settings[USBPD_PORT_COUNT];
extern USBPD_IdSettingsTypeDef    DPM_ID_Settings[USBPD_PORT_COUNT];
extern USBPD_USER_SettingsTypeDef DPM_USER_Settings[USBPD_PORT_COUNT];
#else /* USBPD_DPM_CORE_C */  // include in usbpd_dpm_core.c
USBPD_SettingsTypeDef DPM_Settings[USBPD_PORT_COUNT] = {
    {
     //! Supported SOP : SOP, SOP' SOP" SOP'Debug SOP"Debug
        .PE_SupportedSOP = USBPD_SUPPORTED_SOP_SOP,
     //! spec revision value
        .PE_SpecRevision = USBPD_SPECIFICATION_REV3,
     //! Default port role
        .PE_DefaultRole = USBPD_PORTPOWERROLE_SNK,
     //! support port role swap
        .PE_RoleSwap = USBPD_FALSE,
     //! VDM support
        .PE_VDMSupport = USBPD_FALSE,
     //! Can respond successfully to a Discover Identity
        .PE_RespondsToDiscovSOP = USBPD_FALSE,
     //! Can send a Discover Identity
        .PE_AttemptsDiscovSOP = USBPD_FALSE,
     //! support Ping (only for PD3.0)
        .PE_PingSupport = USBPD_FALSE,
     //! support caps counter
        .PE_CapscounterSupport = USBPD_FALSE,
     //! CAD role toggle
        .CAD_RoleToggle = USBPD_FALSE,
     //! CAD try feature
        .CAD_TryFeature = 0,
     //! CAD accessory support
        .CAD_AccesorySupport = USBPD_FALSE,
     //! PD3 SUPPORT FEATURE
        .PE_PD3_Support.d =
            {
                //! support Unchunked mode (valid only spec revision 3.0)
                .PE_UnchunkSupport = USBPD_FALSE,
                //! support fast role swap only spec revision 3.0
                .PE_FastRoleSwapSupport = USBPD_FALSE,
                //! PPS message NOT supported by PE stack
                .Is_GetPPSStatus_Supported = USBPD_FALSE,
                //! Source_Capabilities_Extended message supported or not by DPM
                .Is_SrcCapaExt_Supported = USBPD_FALSE,
                //! Alert message supported or not by DPM
                .Is_Alert_Supported = USBPD_FALSE,
                //! Status message supported or not by DPM
                //! (Is_Alert_Supported should be enabled)
                .Is_GetStatus_Supported = USBPD_FALSE,
                //! Manufacturer_Info message supported or not by DPM
                .Is_GetManufacturerInfo_Supported = USBPD_FALSE,
                //! Country_Codes message supported or not by DPM
                .Is_GetCountryCodes_Supported = USBPD_FALSE,
                //! Country_Info message supported or not by DPM
                .Is_GetCountryInfo_Supported = USBPD_FALSE,
                //! Security_Response message supported or not by DPM
                .Is_SecurityRequest_Supported = USBPD_FALSE,
                //! Firmware update response message supported by PE
                .Is_FirmUpdateRequest_Supported = USBPD_FALSE,
                //! Get Battery Capabitity and Status messages supported by PE
                .Is_GetBattery_Supported = USBPD_FALSE,
            },  //! uint8_t CAD_SRCToggleTime;
        .CAD_SRCToggleTime = 0,
     //! uint8_t CAD_SNKToggleTime;
        .CAD_SNKToggleTime = 0,
     }
};

USBPD_IdSettingsTypeDef DPM_ID_Settings[USBPD_PORT_COUNT] = {
    {
     //! Value provided by the USB-IF assigned to the product
        .XID = USBPD_XID,
     //! Vendor ID (assigned by the USB-IF)
        .VID = USBPD_VID,
     //! Product ID (assigned by the manufacturer)
        .PID = USBPD_PID,
     },
};

USBPD_USER_SettingsTypeDef DPM_USER_Settings[USBPD_PORT_COUNT] = {
    {
     //! support data swap
        .PE_DataSwap = USBPD_FALSE,
     //! support VCONN swap
        .PE_VconnSwap = USBPD_FALSE,
     //!  Support of DR Swap to DFP
        .PE_DR_Swap_To_DFP = USBPD_TRUE,
     //!  Support of DR Swap to UFP
        .PE_DR_Swap_To_UFP = USBPD_TRUE,
     //! SNK Extended Capability
        .DPM_SNKRequestedPower =
            {
                .MaxOperatingCurrentInmAunits = 5000,
                .OperatingVoltageInmVunits    = 20000,
                .MaxOperatingVoltageInmVunits = 20000,
                .MinOperatingVoltageInmVunits = 20000,
                .OperatingPowerInmWunits      = 45000,
                .MaxOperatingPowerInmWunits   = 100000,
            }, .DPM_SNKExtendedCapa =
            {
                //! Vendor ID (assigned by the USB-IF)
                .VID = USBPD_VID,
                //! Product ID (assigned by the manufacturer)
                .PID = USBPD_PID,
                //! Value provided by the USB-IF assigned to the product
                .XID = USBPD_XID,
                //! Firmware version number
                .FW_revision = 1,
                //! Hardware version number
                .HW_revision = 2,
                //! SKEDV Version (not the specification Version)
                //! based on @ref USBPD_SKEDB_VERSION
                .SKEDB_Version = USBPD_SKEDB_VERSION_1P0,
                //! Load Step based on @ref USBPD_SKEDB_LOADSTEP
                .LoadStep = USBPD_SKEDB_LOADSTEP_150MA,
                //! Sink Load Characteristics
                .SinkLoadCharac.b =
                    {
                        //! Percent overload in 10% increments Values higher
                        //! than 25 (11001b) are clipped to 250%.
                        //! 00000b is the default.
                        .PercentOverload = 0,
                        //! Overload period in 20ms when bits 0-4 non-zero
                        .OverloadPeriod = 0,
                        //! Duty Cycle in 5% increments when bits 0-4 are
                        //! non-zero
                        .DutyCycle = 0,
                        //! Can tolerate VBUS Voltage drop
                        .VBusVoltageDrop = 0,
                    },
                //! Compliance based on combination of
                //! @ref USBPD_SKEDB_COMPLIANCE
                .Compliance = 0,
                //! Touch Temp based on @ref USBPD_SKEDB_TOUCHTEMP
                .Touchtemp = USBPD_SKEDB_TOUCHTEMP_NA,
                //! Battery info
                .BatteryInfo = 0,
                //! Sink Modes based on combination of
                //! @ref USBPD_SKEDB_SINKMODES
                .SinkModes = 0,
                //! The Minimum PDP required by the Sink to operate without
                //! consuming any power from its Battery(s) should it have
                //! one
                .SinkMinimumPDP = 0,
                //! The PDP the Sink requires to operate normally. For Sinks
                //! with a Battery, it is the PDP rating of the charger
                //! supplied with it or recommended for it.
                .SinkOperationalPDP = 0,
                //! The Maximum PDP the Sink can consume to operate and
                //! charge its Battery(s) should it have one
                .SinkMaximumPDP = 0,
            },
#if defined(USBPD_REV30_SUPPORT)
#if _MANU_INFO
     //! Manufacturer information used for the port
        .DPM_ManuInfoPort =
            {
                //! Vendor ID (assigned by the USB-IF)
                .VID = USBPD_VID,
                //! Product ID (assigned by the manufacturer)
                .PID = USBPD_PID,
                //! Vendor defined byte array
                .ManuString = "STMicroelectronics",
            },
#endif /* _MANU_INFO */
#endif /* USBPD_REV30_SUPPORT */
     },
};

#endif /* !__USBPD_DPM_CORE_C */

#ifdef __cplusplus
}
#endif
