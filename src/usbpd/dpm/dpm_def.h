#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <usbpd_def.h>

/** @brief  USBPD PDO Selection method enum definition
 *
 */
typedef enum
{
    PDO_SEL_METHOD_MAX_PWR,
    PDO_SEL_METHOD_MIN_PWR,
    PDO_SEL_METHOD_MAX_VOLT,
    PDO_SEL_METHOD_MIN_VOLT,
    PDO_SEL_METHOD_MAX_CUR,
    PDO_SEL_METHOD_MIN_CUR
} USBPD_DPM_PDO_SelectionMethodTypeDef;

/**
 * @brief  USBPD DPM handle Structure definition
 * @{
 */
typedef struct
{
    //! The list of received Source Power Data Objects from Port partner
    uint32_t DPM_ListOfRcvSRCPDO[USBPD_MAX_NB_PDO];
    //! The number of received Source Power Data Objects from port Partner
    // (when Port partner is a Source or a DRP port
    uint32_t DPM_NumberOfRcvSRCPDO;
    //! Received request Power Data Object message from the port Partner
    uint32_t DPM_RcvRequestDOMsg;
    //! Previous Request Power Data Object message to be sent
    uint32_t DPM_RequestDOMsgPrevious;

    //! SNK Extended Capability received by port partner
    USBPD_SKEDB_TypeDef DPM_RcvSNKExtendedCapa;

    //! Request Power Data Object message to be sent
    uint32_t DPM_RequestDOMsg;
    //! RDO Position of requested DO in Source list of capabilities
    uint32_t DPM_RDOPosition;
    //! RDO Position of previous requested DO in Source list of capabilities
    uint32_t DPM_RDOPositionPrevious;
    //! Value of requested voltage
    uint32_t DPM_RequestedVoltage;
    //! Value of requested current
    uint32_t DPM_RequestedCurrent;
} USBPD_HandleTypeDef;

typedef struct
{
    //! support data swap
    uint32_t PE_DataSwap: 1U;
    //! support VCONN swap
    uint32_t PE_VconnSwap: 1U;
    //! If supported, DR Swap to DFP can be accepted or not by the user else
    //! directly rejected
    uint32_t PE_DR_Swap_To_DFP: 1U;
    //! If supported, DR Swap to UFP can be accepted or not by the user else
    //! directly rejected
    uint32_t PE_DR_Swap_To_UFP: 1U;
    //! Reserved bits
    uint32_t Reserved1: 28U;
    //! Requested Power by the sink board
    USBPD_SNKPowerRequest_TypeDef DPM_SNKRequestedPower;
    //! Manufacturer information used for the port
    USBPD_MIDB_TypeDef DPM_ManuInfoPort;
    //! SNK Extended Capability
    USBPD_SKEDB_TypeDef DPM_SNKExtendedCapa;
    //! Reserved bits to match with Manufacturer information
    uint16_t ReservedManu;
} USBPD_USER_SettingsTypeDef;

typedef struct
{
    //! Value provided by the USB-IF assigned to the product
    uint32_t XID;
    //! Vendor ID (assigned by the USB-IF)
    uint16_t VID;
    //! Product ID (assigned by the manufacturer)
    uint16_t PID;
} USBPD_IdSettingsTypeDef;

// !< No match found between Received SRC PDO and SNK capabilities
#define DPM_NO_SRC_PDO_FOUND 0xFFU

#ifdef __cplusplus
}
#endif
