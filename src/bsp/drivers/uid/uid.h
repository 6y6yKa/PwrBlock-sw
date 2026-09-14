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

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Read MCU UID
 *
 * @param read_buf - 96 bit stored in system memory
 * @param read_buf_len - length of read buffer
 * @return size_t - lengh of UID in bytes
 */
size_t uid_mcu_get(uint8_t read_buf[], size_t read_buf_len);


/**
 * @brief Get USB Serial number string from unique ID if available. Return
 * number of character. Input is string descriptor from index 1
 * (index 0 is type + len)
 *
 * @param desc_str1
 * @param max_chars
 * @return size_t
 */
size_t uid_usb_get(uint16_t read_buf[], size_t read_buf_len);
