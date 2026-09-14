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

#include <cmsis_os2.h>

/**
 * @brief Read UID (MAC) from EERPOM.
 *
 * @param dst Pointer to store result.
 * @return osStatus_t osOk - reading done, osError - error on rx/tx with EEPROM.
 */
osStatus_t storage_mac_get(uint8_t* dst);

/**
 * @brief Read block of data from storage.
 *
 * @param addr Start address of block.
 * @param dst Destination data pointer.
 * @param size Number of bytes to be read.
 * @return osStatus_t osOk - reading done, osError - error on rx/tx with EEPROM.
 */
osStatus_t storage_read(uint8_t addr, uint8_t* dst, uint16_t size);

/**
 * @brief Write block of data to storage.
 *
 * @param addr Start address of block.
 * @param src Source data pointer.
 * @param size Number of bytes to be write.
 * @return osStatus_t osOk - writing done, osError - error on rx/tx with EEPROM.
 */
osStatus_t storage_write(uint8_t addr, uint8_t* src, uint16_t size);
