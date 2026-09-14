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

#include "calib_const.h"

#include "storage.h"
#include "storage_def.h"

const float CALIB_CONST_DEFAULT_SLOPE  = 1.0F;
const float CALIB_CONST_DEFAULT_OFFSET = 0.0F;

static void calib_const_set_defaults(CalibConst_t* instance)
{
    instance->val.v_slope  = CALIB_CONST_DEFAULT_SLOPE;
    instance->val.v_offset = CALIB_CONST_DEFAULT_OFFSET;
    instance->val.i_slope  = CALIB_CONST_DEFAULT_SLOPE;
    instance->val.i_offset = CALIB_CONST_DEFAULT_OFFSET;
}

osStatus_t calib_const_init(CalibConst_t* instance)
{
    uint8_t addr = STORAGE_CALIB_CONST_START;
    uint8_t cfg;
    if (storage_read(addr, &cfg, STORAGE_IS_INIT_FLAG_SIZE) != osOK) {
        // EEPROM read failed: fall back to neutral coeffs so calibration
        // does not scale by stale/uninitialized values
        calib_const_set_defaults(instance);
        return osError;
    };

    if (cfg == STORAGE_IS_INIT_FALSE) {
        // In case of empty (not inited) EEPROM
        calib_const_set_defaults(instance);

        if (calib_const_store(instance) != osOK) {
            return osError;
        };

        cfg = STORAGE_IS_INIT_TRUE;
        if (storage_write(addr, &cfg, STORAGE_IS_INIT_FLAG_SIZE) != osOK) {
            return osError;
        }
    }
    else {
        if (calib_const_recall(instance) != osOK) {
            // EEPROM read failed: fall back to neutral coeffs so
            // calibration does not scale by stale/uninitialized values
            calib_const_set_defaults(instance);
            return osError;
        };
    }

    return osOK;
}

osStatus_t calib_const_store(CalibConst_t* src)
{
    uint8_t addr = STORAGE_CALIB_CONST_START + STORAGE_IS_INIT_FLAG_SIZE;
    return storage_write(addr, src->raw, sizeof(CalibConstProperties_t));
}

osStatus_t calib_const_recall(CalibConst_t* dst)
{
    uint8_t addr = STORAGE_CALIB_CONST_START + STORAGE_IS_INIT_FLAG_SIZE;
    return storage_read(addr, dst->raw, sizeof(CalibConstProperties_t));
}
