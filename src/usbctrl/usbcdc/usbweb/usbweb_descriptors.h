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
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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

#pragma once

#include <common/tusb_types.h>
#include <device/usbd.h>
#include <stdint.h>

enum
{
    VENDOR_REQUEST_WEBUSB    = 1,
    VENDOR_REQUEST_MICROSOFT = 2
};

//--------------------------------------------------------------------+
// WebUSB vendor descriptor
//--------------------------------------------------------------------+

#define URL "hackaday.io/project/204044-pwrblock-323"

const struct TU_ATTR_PACKED
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bScheme;
    char    url[sizeof(URL) - 1];
} desc_url = {
    .bLength         = 3 + sizeof(URL) - 1,
    .bDescriptorType = 3,  // WEBUSB URL type
    .bScheme         = 1,  // 0: http, 1: https
    .url             = URL
};

//--------------------------------------------------------------------+
// BOS Descriptor
//--------------------------------------------------------------------+

/* Microsoft OS 2.0 registry property descriptor
Per MS requirements
https://msdn.microsoft.com/en-us/library/windows/hardware/hh450799(v=vs.85).aspx
device should create DeviceInterfaceGUIDs. It can be done by driver and
in case of real PnP solution device should expose MS "Microsoft OS 2.0
registry property descriptor". Such descriptor can insert any record
into Windows registry per device/configuration/interface. In our case it
will insert "DeviceInterfaceGUIDs" multistring property.

GUID is freshly generated and should be OK to use.

https://developers.google.com/web/fundamentals/native-hardware/build-for-webusb/
(Section Microsoft OS compatibility descriptors)
*/

#define BOS_TOTAL_LEN                                                          \
    (TUD_BOS_DESC_LEN + TUD_BOS_WEBUSB_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)

#define MS_OS_20_DESC_LEN 0xB2

// BOS Descriptor is required for webUSB
const uint8_t desc_bos[] = {
    // total length, number of device caps
    TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, 2),

    // Vendor Code, iLandingPage
    TUD_BOS_WEBUSB_DESCRIPTOR(VENDOR_REQUEST_WEBUSB, 1),

    // Microsoft OS 2.0 descriptor
    TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, VENDOR_REQUEST_MICROSOFT)
};

const uint8_t* tud_descriptor_bos_cb(void)
{
    return desc_bos;
}

const uint8_t DESC_MS_OS_20[] = {
    // Set header: length, type, windows version, total length
    U16_TO_U8S_LE(0x000A),
    U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR),
    U32_TO_U8S_LE(0x06030000),
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

    // Configuration subset header: length, type, configuration index, reserved,
    // configuration total length
    U16_TO_U8S_LE(0x0008),
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION),
    0,
    0,
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A),

    // Function Subset header: length, type, first interface, reserved, subset
    // length
    U16_TO_U8S_LE(0x0008),
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION),
    2,
    0,
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08),

    // MS OS 2.0 Compatible ID descriptor: length, type, compatible ID, sub
    // compatible ID
    U16_TO_U8S_LE(0x0014),
    U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID),
    'W',
    'I',
    'N',
    'U',
    'S',
    'B',
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,  // sub-compatible

    // MS OS 2.0 Registry property descriptor: length, type
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08 - 0x08 - 0x14),
    U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
    U16_TO_U8S_LE(0x0007),
    U16_TO_U8S_LE(0x002A),  // wPropertyDataType, wPropertyNameLength and
                            // PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
    'D',
    0x00,
    'e',
    0x00,
    'v',
    0x00,
    'i',
    0x00,
    'c',
    0x00,
    'e',
    0x00,
    'I',
    0x00,
    'n',
    0x00,
    't',
    0x00,
    'e',
    0x00,
    'r',
    0x00,
    'f',
    0x00,
    'a',
    0x00,
    'c',
    0x00,
    'e',
    0x00,
    'G',
    0x00,
    'U',
    0x00,
    'I',
    0x00,
    'D',
    0x00,
    's',
    0x00,
    0x00,
    0x00,
    U16_TO_U8S_LE(0x0050),  // wPropertyDataLength
    // bPropertyData: “{975F44D9-0D08-43FD-8B3E-127CA8AFFF9D}”.
    '{',
    0x00,
    '9',
    0x00,
    '7',
    0x00,
    '5',
    0x00,
    'F',
    0x00,
    '4',
    0x00,
    '4',
    0x00,
    'D',
    0x00,
    '9',
    0x00,
    '-',
    0x00,
    '0',
    0x00,
    'D',
    0x00,
    '0',
    0x00,
    '8',
    0x00,
    '-',
    0x00,
    '4',
    0x00,
    '3',
    0x00,
    'F',
    0x00,
    'D',
    0x00,
    '-',
    0x00,
    '8',
    0x00,
    'B',
    0x00,
    '3',
    0x00,
    'E',
    0x00,
    '-',
    0x00,
    '1',
    0x00,
    '2',
    0x00,
    '7',
    0x00,
    'C',
    0x00,
    'A',
    0x00,
    '8',
    0x00,
    'A',
    0x00,
    'F',
    0x00,
    'F',
    0x00,
    'F',
    0x00,
    '9',
    0x00,
    'D',
    0x00,
    '}',
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};

TU_VERIFY_STATIC(sizeof(DESC_MS_OS_20) == MS_OS_20_DESC_LEN, "Incorrect size");
