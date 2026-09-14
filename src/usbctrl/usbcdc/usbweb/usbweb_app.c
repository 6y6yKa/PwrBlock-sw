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

#include <tusb.h>

#include "usbweb_descriptors.h"

// Invoked when a control transfer occurred on an interface of this class
// Driver response accordingly to the request and the transfer stage
// (setup/data/ack) return false to stall control endpoint (e.g unsupported
// request)
bool tud_vendor_control_xfer_cb(
    uint8_t rhport, uint8_t stage, const tusb_control_request_t* request
)
{
    // nothing to with DATA & ACK stage
    if (stage != CONTROL_STAGE_SETUP) return true;

    switch (request->bmRequestType_bit.type) {
        case TUSB_REQ_TYPE_VENDOR:
            switch (request->bRequest) {
                case VENDOR_REQUEST_WEBUSB:
                    // match vendor request in BOS descriptor
                    // Get landing page url
                    return tud_control_xfer(
                        rhport,
                        request,
                        (void*)(uintptr_t)&desc_url,
                        desc_url.bLength
                    );

                case VENDOR_REQUEST_MICROSOFT:
                    if (request->wIndex == 7) {
                        // Get Microsoft OS 2.0 compatible descriptor
                        uint16_t total_len;
                        memcpy(&total_len, DESC_MS_OS_20 + 8, 2);

                        return tud_control_xfer(
                            rhport,
                            request,
                            (void*)(uintptr_t)DESC_MS_OS_20,
                            total_len
                        );
                    }
                    else {
                        return false;
                    }

                default: break;
            }
            break;

        default: break;
    }

    // stall unknown request
    return false;
}
