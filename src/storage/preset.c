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

#include "preset.h"

#include "storage.h"
#include "storage_def.h"

static Preset_t preset[STORAGE_PRESETS_MAX_NUM];

const uint32_t DEFAULT_V_PRESET = 5000;
const uint32_t DEFAULT_I_PRESET = 500;

// Private prototypes
static osStatus_t preset_storage_store(Preset_t* src, uint8_t preset_id);
static osStatus_t preset_storage_recall(Preset_t* dst, uint8_t preset_id);
static void       preset_set_defaults(Preset_t* dst);

osStatus_t preset_init(void)
{
    uint8_t addr = STORAGE_PRESETS_START;
    uint8_t cfg;
    if (storage_read(addr, &cfg, STORAGE_IS_INIT_FLAG_SIZE) != osOK) {
        // EEPROM read failed: fall back to defaults so the cache does not
        // hold stale/uninitialized values
        for (size_t i = 0; i < STORAGE_PRESETS_MAX_NUM; i++) {
            preset_set_defaults(&preset[i]);
        }
        return osError;
    };

    if (cfg == STORAGE_IS_INIT_TRUE) {
        osStatus_t status = osOK;
        for (size_t i = 0; i < STORAGE_PRESETS_MAX_NUM; i++) {
            if (preset_storage_recall(&preset[i], i) != osOK) {
                // EEPROM read failed: fall back to defaults so the cache
                // does not hold stale/uninitialized values
                preset_set_defaults(&preset[i]);
                status = osError;
            };
        }
        if (status != osOK) {
            return status;
        }
    }
    else {
        // In case of empty (not inited) EEPROM
        for (size_t i = 0; i < STORAGE_PRESETS_MAX_NUM; i++) {
            preset_set_defaults(&preset[i]);

            if (preset_storage_store(&preset[i], i) != osOK) {
                return osError;
            };
        }

        cfg = STORAGE_IS_INIT_TRUE;
        if (storage_write(addr, &cfg, STORAGE_IS_INIT_FLAG_SIZE) != osOK) {
            return osError;
        }
    }

    return osOK;
}

osStatus_t preset_write(Preset_t* src, uint8_t preset_id)
{
    if (--preset_id < STORAGE_PRESETS_MAX_NUM) {
        // Update cached data
        preset[preset_id].cell.v_out = src->cell.v_out;
        preset[preset_id].cell.i_out = src->cell.i_out;
        // Store to memory
        return preset_storage_store(&preset[preset_id], preset_id);
    }
    // Else
    return osErrorParameter;
}

osStatus_t preset_read(Preset_t* dst, uint8_t preset_id)
{
    if (--preset_id < STORAGE_PRESETS_MAX_NUM) {
        // Update from cached data
        dst->cell.v_out = preset[preset_id].cell.v_out;
        dst->cell.i_out = preset[preset_id].cell.i_out;
        return osOK;
    }
    // Else
    return osErrorParameter;
}

static osStatus_t preset_storage_store(Preset_t* src, uint8_t preset_id)
{
    uint8_t addr = STORAGE_PRESETS_START + STORAGE_IS_INIT_FLAG_SIZE +
                   preset_id * sizeof(PresetProperties_t);
    return storage_write(addr, src->raw, sizeof(PresetProperties_t));
}

static osStatus_t preset_storage_recall(Preset_t* dst, uint8_t preset_id)
{
    uint8_t addr = STORAGE_PRESETS_START + STORAGE_IS_INIT_FLAG_SIZE +
                   preset_id * sizeof(PresetProperties_t);
    return storage_read(addr, dst->raw, sizeof(PresetProperties_t));
}

static void preset_set_defaults(Preset_t* dst)
{
    dst->cell.v_out = DEFAULT_V_PRESET;
    dst->cell.i_out = DEFAULT_I_PRESET;
}
