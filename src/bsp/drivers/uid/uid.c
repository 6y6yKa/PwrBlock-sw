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

#include "uid.h"

#include <stm32g0xx_hal.h>

size_t uid_mcu_get(uint8_t read_buf[], size_t read_buf_len)
{
    const uint8_t UID_LEN = 12;
    if (read_buf_len < UID_LEN) {
        return 0;
    }

    volatile uint32_t* stm32_uuid = (volatile uint32_t*)UID_BASE;
    uint32_t*          id32       = (uint32_t*)(uintptr_t)read_buf;

    id32[0] = stm32_uuid[0];
    id32[1] = stm32_uuid[1];
    id32[2] = stm32_uuid[2];

    return UID_LEN;
}

size_t uid_usb_get(uint16_t read_buf[], size_t read_buf_len)
{
    uint8_t uid[12];
    size_t  uid_len;

    uid_len = uid_mcu_get(uid, sizeof(uid));

    if (uid_len > read_buf_len / 2) {
        uid_len = read_buf_len / 2;
    }

    for (size_t i = 0; i < uid_len; i++) {
        for (size_t j = 0; j < 2; j++) {
            const char NIBBLE_TO_HEX[16] = {
                '0',
                '1',
                '2',
                '3',
                '4',
                '5',
                '6',
                '7',
                '8',
                '9',
                'A',
                'B',
                'C',
                'D',
                'E',
                'F'
            };
            const uint8_t NIBBLE      = (uid[i] >> (j * 4)) & 0xf;
            read_buf[i * 2 + (1 - j)] = NIBBLE_TO_HEX[NIBBLE];  // UTF-16-LE
        }
    }

    return 2 * uid_len;
}
