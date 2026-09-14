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

#include <bsp/drivers/uid/uid.h>
#include <tusb.h>

/* A combination of interfaces must have a unique product id, since PC will save
 * device driver after the first plug. Same VID/PID with different interface e.g
 * MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]       MIDI | HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n) ((CFG_TUD_##itf) << (n))

/* Example of PID definition:
#define USB_PID \
    (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
    _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4))
*/

// TODO: update PID (assigned by the USB-IF), current only for testing
#define USB_PID (0x0001)

#define USB_VID 0x1209
#define USB_BCD 0x0210  // at least 2.1 or 3.x for BOS & webUSB
//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
const tusb_desc_device_t desc_device = {
    .bLength         = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB          = USB_BCD,
    // Use Interface Association Descriptor (IAD) for CDC
    // As required by USB Specs IAD's subclass must be common class (2) and
    // protocol must be IAD (1)
    .bDeviceClass    = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor  = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct      = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
const uint8_t* tud_descriptor_device_cb(void)
{
    return (const uint8_t*)&desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
enum
{
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_VENDOR,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN                                                       \
    (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_VENDOR_DESC_LEN)

#if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X ||                                      \
    CFG_TUSB_MCU == OPT_MCU_LPC177X_8X || CFG_TUSB_MCU == OPT_MCU_LPC40XX
// LPC 17xx and 40xx endpoint type (bulk/interrupt/iso) are fixed by its number
// 0 control, 1 In, 2 Bulk, 3 Iso, 4 In etc ...
#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x82

#define EPNUM_VENDOR_OUT 0x05
#define EPNUM_VENDOR_IN  0x85

#elif CFG_TUSB_MCU == OPT_MCU_CXD56
// CXD56 USB driver has fixed endpoint type (bulk/interrupt/iso) and direction
// (IN/OUT) by its number 0 control (IN/OUT), 1 Bulk (IN), 2 Bulk (OUT), 3 In
// (IN), 4 Bulk (IN), 5 Bulk (OUT), 6 In (IN)
#define EPNUM_CDC_NOTIF 0x83
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x81

#define EPNUM_VENDOR_OUT 0x05
#define EPNUM_VENDOR_IN  0x84

#elif defined(TUD_ENDPOINT_ONE_DIRECTION_ONLY)
// MCUs that don't support a same endpoint number with different direction IN
// and OUT defined in tusb_mcu.h
//    e.g EP1 OUT & EP1 IN cannot exist together
#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x83

#define EPNUM_VENDOR_OUT 0x04
#define EPNUM_VENDOR_IN  0x85

#else
#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x82

#define EPNUM_VENDOR_OUT 0x03
#define EPNUM_VENDOR_IN  0x83
#endif

const uint8_t desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // Interface number, string index, EP notification address and size, EP data
    // address (out, in) and size.
    TUD_CDC_DESCRIPTOR(
        ITF_NUM_CDC,
        4,
        EPNUM_CDC_NOTIF,
        8,
        EPNUM_CDC_OUT,
        0x80 | EPNUM_CDC_IN,
        TUD_OPT_HIGH_SPEED ? 512 : 64
    ),

    // Interface number, string index, EP Out & IN address, EP size
    TUD_VENDOR_DESCRIPTOR(
        ITF_NUM_VENDOR,
        5,
        EPNUM_VENDOR_OUT,
        0x80 | EPNUM_VENDOR_IN,
        TUD_OPT_HIGH_SPEED ? 512 : 64
    )
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
const uint8_t* tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;  // for multiple configurations
    return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// String Descriptor Index
enum
{
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
};

// array of pointer to string descriptors
const char* string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "TinyUSB", // 1: Manufacturer
    "TinyUSB Device", // 2: Product
    NULL, // 3: Serials will use unique ID if possible
    "TinyUSB CDC", // 4: CDC Interface
    "TinyUSB WebUSB"  // 5: Vendor Interface
};

static uint16_t _desc_str[32 + 1];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
const uint16_t* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;
    size_t chr_count;

    switch (index) {
        case STRID_LANGID:
            memcpy(&_desc_str[1], string_desc_arr[0], 2);
            chr_count = 1;
            break;

        case STRID_SERIAL: chr_count = uid_usb_get(_desc_str + 1, 32); break;

        default:
            // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
            // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

            if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
                return NULL;

            const char* str = string_desc_arr[index];

            // Cap at max char
            chr_count              = strlen(str);
            const size_t max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) -
                                     1;  // -1 for string type
            if (chr_count > max_count) chr_count = max_count;

            // Convert ASCII string into UTF-16
            for (size_t i = 0; i < chr_count; i++) {
                _desc_str[1 + i] = str[i];
            }
            break;
    }

    // first byte is length (including header), second byte is string type
    _desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

    return _desc_str;
}
