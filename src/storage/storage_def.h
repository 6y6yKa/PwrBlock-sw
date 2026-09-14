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

#include "calib_const.h"
#include "preset.h"

// Storage IS_INIT flag properties
#define STORAGE_IS_INIT_FLAG_SIZE (sizeof(uint8_t))
#define STORAGE_IS_INIT_FALSE     (0xFFU)
#define STORAGE_IS_INIT_TRUE      (0x01U)

// === Memory map ==
typedef enum
{
    STORAGE_PRESETS_START   = 0,
    STORAGE_PRESETS_MAX_NUM = 4U,
    STORAGE_PRESETS_SIZE =
        STORAGE_IS_INIT_FLAG_SIZE +
        (sizeof(PresetProperties_t) * STORAGE_PRESETS_MAX_NUM),

    STORAGE_CALIB_CONST_START = STORAGE_PRESETS_START + STORAGE_PRESETS_SIZE,
    STORAGE_CALIB_CONST_SIZE =
        STORAGE_IS_INIT_FLAG_SIZE + sizeof(CalibConstProperties_t),

    STORAGE_TOTAL_SIZE = STORAGE_CALIB_CONST_START + STORAGE_CALIB_CONST_SIZE
} StorageMemoryMap_t;
