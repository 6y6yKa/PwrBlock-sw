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

typedef struct __attribute__((packed))
{
    uint32_t v_out;
    uint32_t i_out;
} PresetProperties_t;

typedef union __attribute__((aligned(4)))
{
    PresetProperties_t cell;
    uint8_t            raw[sizeof(PresetProperties_t)];
} Preset_t;

/**
 * @brief Init presets cache.
 *
 * @return osStatus_t osOk - init done, osError - erron on rx/tx with EEPROM.
 */
osStatus_t preset_init(void);

/**
 * @brief Write preset.
 *
 * @param src Pointer to preset instance.
 * @param preset_id Preset ID (parameter in human frendly format [1 to N]
 * @return osStatus_t osOk - writing done, osError - error on rx/tx with EEPROM.
 */
osStatus_t preset_write(Preset_t* src, uint8_t preset_id);

/**
 * @brief Read preset.
 *
 * @param dst Pointer to preset instance.
 * @param preset_id Preset ID (parameter in human frendly format [1 to N]
 * @return osStatus_t osOk - reading done, osError - error on rx/tx with EEPROM.
 */
osStatus_t preset_read(Preset_t* dst, uint8_t preset_id);
