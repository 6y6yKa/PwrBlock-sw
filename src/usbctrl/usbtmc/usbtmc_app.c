/*
 * Copyright (c) 2026 Everypin
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
 *
 * --- NOTICE OF MODIFICATION ---
 * This file has been modified by Everypin in 2026.
 * Original code is licensed under the MIT License (see below).
 * Modifications are licensed under the Apache License 2.0.
 *
 * --- ORIGINAL MIT LICENSE NOTE ---
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Nathan Conrad
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "indication/indication.h"
#include "scpi/scpi_def.h"
#include "threads/timestamp.h"
#include "tusb.h"
#include <stdlib.h> /* atoi */
#include <string.h>

#if (CFG_TUD_USBTMC_ENABLE_488)
static const usbtmc_response_capabilities_488_t
#else
static const usbtmc_response_capabilities_t
#endif
    tud_usbtmc_app_capabilities = {
        .USBTMC_status = USBTMC_STATUS_SUCCESS,
        .bcdUSBTMC     = USBTMC_VERSION,
        .bmIntfcCapabilities =
            {.listenOnly = 0, .talkOnly = 0, .supportsIndicatorPulse = 1},
        .bmDevCapabilities = {.canEndBulkInOnTermChar = 0},

#if (CFG_TUD_USBTMC_ENABLE_488)
        .bcdUSB488 = USBTMC_488_VERSION,
        .bmIntfcCapabilities488 =
            {.supportsTrigger = 1, .supportsREN_GTL_LLO = 0, .is488_2 = 1},
        .bmDevCapabilities488 =
            {
                                  .SCPI = 1,
                                  .SR1  = 0,
                                  .RL1  = 0,
                                  .DT1  = 0,
                                  }
#endif
};

// TODO: Remove, defined in scpi_parser
#define IEEE4882_STB_QUESTIONABLE (0x08u)
#define IEEE4882_STB_MAV          (0x10u)
#define IEEE4882_STB_SER          (0x20u)
#define IEEE4882_STB_SRQ          (0x40u)

// static const char idn[] = "TinyUSB,ModelNumber,SerialNumber,FirmwareVer and a
// bunch of other text to make it longer than a packet, perhaps? lets make it
// three transfers...\n";
static volatile uint8_t status;

// 0=not query, 1=queried, 2=delay,set(MAV), 3=delay 4=ready?
// (to simulate delay)
static volatile uint16_t queryState = 0;
static volatile uint32_t queryDelayStart;
static volatile uint32_t bulkInStarted;
static volatile uint32_t idnQuery;

static uint32_t resp_delay =
    125u;  // Adjustable delay, to allow for better testing
static size_t buffer_out_len;
static size_t buffer_tx_ix;  // for transmitting using multiple transfers
// host centric naming: out - PC to device, in - device to port
static uint8_t buffer_out[225];  // A few packets long should be enough.
uint8_t        buffer_in[225];   // A few packets long should be enough.
size_t         buffer_in_len;

void tud_usbtmc_open_cb(uint8_t interface_id)
{
    (void)interface_id;
    tud_usbtmc_start_bus_read();
}

#if (CFG_TUD_USBTMC_ENABLE_488)
const usbtmc_response_capabilities_488_t*
#else
const usbtmc_response_capabilities_t*
#endif
tud_usbtmc_get_capabilities_cb()
{
    return &tud_usbtmc_app_capabilities;
}

bool tud_usbtmc_msg_trigger_cb(usbtmc_msg_generic_t* msg)
{
    (void)msg;
    // Let trigger set the SRQ
    status |= IEEE4882_STB_SRQ;
    return true;
}

bool tud_usbtmc_msgBulkOut_start_cb(
    const usbtmc_msg_request_dev_dep_out* msgHeader
)
{
    (void)msgHeader;
    buffer_out_len = 0;
    if (msgHeader->TransferSize > sizeof(buffer_out)) {

        return false;
    }
    return true;
}

bool tud_usbtmc_msg_data_cb(void* data, size_t len, bool transfer_complete)
{
    // If transfer isn't finished, we just ignore it (for now)

    if (len + buffer_out_len < sizeof(buffer_out)) {
        memcpy(&(buffer_out[buffer_out_len]), data, len);
        buffer_out_len += len;
    }
    else {
        return false;  // buffer overflow!
    }
    queryState = transfer_complete;
    idnQuery   = 0;

    if (transfer_complete) {
        SCPI_Input(&scpi_context, (char*)buffer_out, buffer_out_len);
        idnQuery = 1;
    }

    if (transfer_complete &&
        (!strncmp("delay ", data, 5) || !strncmp("DELAY ", data, 5))) {
        queryState = 0;
        int d      = atoi((char*)data + 5);
        if (d > 10000) d = 10000;
        if (d < 0) d = 0;
        resp_delay = (uint32_t)d;
    }
    tud_usbtmc_start_bus_read();
    return true;
}

bool tud_usbtmc_msgBulkIn_complete_cb()
{
    if ((buffer_tx_ix == buffer_out_len) || idnQuery)  // done
    {
        status &= (uint8_t) ~(IEEE4882_STB_MAV);  // clear MAV
        queryState    = 0;
        bulkInStarted = 0;
        buffer_tx_ix  = 0;
        buffer_in_len = 0;
    }
    tud_usbtmc_start_bus_read();

    return true;
}

static unsigned int msgReqLen;

bool tud_usbtmc_msgBulkIn_request_cb(
    const usbtmc_msg_request_dev_dep_in* request
)
{
    msgReqLen = request->TransferSize;

#ifdef xDEBUG
    uart_tx_str_sync("MSG_IN_DATA: Requested!\r\n");
#endif
    if (queryState == 0 || (buffer_tx_ix == 0)) {
        TU_ASSERT(bulkInStarted == 0);
        bulkInStarted = 1;

        // > If a USBTMC interface receives a Bulk-IN request prior to receiving
        // a USBTMC command message
        //   that expects a response, the device must NAK the request (*not
        //   stall*)
    }
    else {
        size_t txlen = tu_min32(buffer_out_len - buffer_tx_ix, msgReqLen);
        tud_usbtmc_transmit_dev_msg_data(
            &buffer_out[buffer_tx_ix],
            txlen,
            (buffer_tx_ix + txlen) == buffer_out_len,
            false
        );
        buffer_tx_ix += txlen;
    }
    // Always return true indicating not to stall the EP.
    return true;
}

void usbtmc_app_task_iter(void)
{
    switch (queryState) {
        case 0: break;
        case 1:
            queryDelayStart = get_millis();
            queryState      = 2;
            break;
        case 2:
            if ((get_millis() - queryDelayStart) > resp_delay) {
                queryDelayStart = get_millis();
                queryState      = 3;
                status |= IEEE4882_STB_MAV;  // MAV
                status |= IEEE4882_STB_SRQ;  // SRQ
            }
            break;
        case 3:
            if ((get_millis() - queryDelayStart) > resp_delay) {
                queryState = 4;
            }
            break;
        case 4:  // time to transmit;
            if (bulkInStarted && (buffer_tx_ix == 0)) {
                tud_usbtmc_transmit_dev_msg_data(
                    buffer_in, buffer_in_len, true, false
                );
                queryState    = 0;
                bulkInStarted = 0;
                // MAV is cleared in the transfer complete callback.
            }
            break;
        default: TU_ASSERT(false, );
    }
}

bool tud_usbtmc_initiate_clear_cb(uint8_t* tmcResult)
{
    *tmcResult    = USBTMC_STATUS_SUCCESS;
    queryState    = 0;
    bulkInStarted = false;
    status        = 0;
    return true;
}

bool tud_usbtmc_check_clear_cb(usbtmc_get_clear_status_rsp_t* rsp)
{
    queryState                   = 0;
    bulkInStarted                = false;
    status                       = 0;
    buffer_tx_ix                 = 0u;
    buffer_out_len               = 0u;
    buffer_in_len                = 0u;
    rsp->USBTMC_status           = USBTMC_STATUS_SUCCESS;
    rsp->bmClear.BulkInFifoBytes = 0u;
    return true;
}

bool tud_usbtmc_initiate_abort_bulk_in_cb(uint8_t* tmcResult)
{
    bulkInStarted = 0;
    *tmcResult    = USBTMC_STATUS_SUCCESS;
    return true;
}

bool tud_usbtmc_check_abort_bulk_in_cb(usbtmc_check_abort_bulk_rsp_t* rsp)
{
    (void)rsp;
    tud_usbtmc_start_bus_read();
    return true;
}

bool tud_usbtmc_initiate_abort_bulk_out_cb(uint8_t* tmcResult)
{
    *tmcResult = USBTMC_STATUS_SUCCESS;
    return true;
}

bool tud_usbtmc_check_abort_bulk_out_cb(usbtmc_check_abort_bulk_rsp_t* rsp)
{
    (void)rsp;
    tud_usbtmc_start_bus_read();
    return true;
}

void tud_usbtmc_bulkIn_clearFeature_cb(void) {}

void tud_usbtmc_bulkOut_clearFeature_cb(void)
{
    tud_usbtmc_start_bus_read();
}

// Return status byte, but put the transfer result status code in the rspResult
// argument.
uint8_t tud_usbtmc_get_stb_cb(uint8_t* tmcResult)
{
    uint8_t old_status = status;
    status             = (uint8_t)(status & ~(IEEE4882_STB_SRQ));  // clear SRQ

    *tmcResult = USBTMC_STATUS_SUCCESS;
    // Increment status so that we see different results on each read...

    return old_status;
}

bool tud_usbtmc_indicator_pulse_cb(
    const tusb_control_request_t* msg, uint8_t* tmcResult
)
{
    (void)msg;
    *tmcResult = USBTMC_STATUS_SUCCESS;
    return true;
}
