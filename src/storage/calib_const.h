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

#include <stdbool.h>

#include <cmsis_os2.h>

typedef struct __attribute__((packed))
{
    float v_slope;
    float v_offset;
    float i_slope;
    float i_offset;
} CalibConstProperties_t;

typedef union __attribute__((aligned(4)))
{
    CalibConstProperties_t val;
    uint8_t                raw[sizeof(CalibConstProperties_t)];
} CalibConst_t;

/**
 * @brief Init calibration consts cache.
 *
 * @param instance - calibration consts buffer instance.
 * @return osStatus_t osOk - init done, osError - erron on rx/tx with EEPROM.
 */
osStatus_t calib_const_init(CalibConst_t* instance);

/**
 * @brief Store calibration consts to nonvolatile mem.
 *
 * @param src - pointer to source consts instance.
 * @return osStatus_t osOk - storing done, osError - error on rx/tx with
 * mem.
 */
osStatus_t calib_const_store(CalibConst_t* src);

/**
 * @brief Recall calibration consts from nonvolatile mem.
 *
 * @param dst - pointer do destination consts instance.
 * @return osStatus_t osOk -recalling done, osError - error on rx/tx with mem.
 */
osStatus_t calib_const_recall(CalibConst_t* dst);
