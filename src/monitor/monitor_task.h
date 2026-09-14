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
#include <stdint.h>

void monitor_app_task(void* argument);

/**
 * @brief Safe getter of Vbus voltage (output).
 *
 * @return uint32_t Value in millivolts.
 */
uint32_t monitor_vsbus_get(void);

/**
 * @brief Safe getter of current flowing via shunt (output).
 *
 * @return uint32_t Value in milliamperes.
 */
uint32_t monitor_current_get(void);

/**
 * @brief Safe getter of Vshunt voltage (voltage drop on Rshunt).
 *
 * @return uint32_t Value in millivolts.
 */
uint32_t monitor_vshunt_get(void);

/**
 * @brief Safe getter of calculated power output.
 *
 * @return uint32_t Value in milliwatts.
 */
uint32_t monitor_power_get(void);

/**
 * @brief Safe getter of calclulated temperature of die.
 *
 * @return uint32_t Value in millidegrees Celsius.
 */
uint32_t monitor_dietemp_get(void);

/**
 * @brief Setter for fuse mode state.
 *
 * @param state True - fuse on, false - fuse off.
 */
void monitor_fuse_state_set(bool state);

/**
 * @brief Getter of fuse mode state.
 *
 * @return true Fuse on.
 * @return false Fuse off.
 */
bool monitor_fuse_state_get(void);

/**
 * @brief Setter for fuse current limit.
 *
 * @param val Value in milliamperes.
 */
void monitor_fuse_current_set(uint32_t val);

/**
 * @brief Getter of fuse current limit.
 *
 * @return uint32_t Value in milliamperes.
 */
uint32_t monitor_fuse_current_get(void);

/**
 * @brief Getter of fuse current min limit.
 *
 * @return uint32_t Value in milliamperes.
 */
uint32_t monitor_fuse_current_min_get(void);

/**
 * @brief Getter of fuse current max limit.
 *
 * @return uint32_t Value in milliamperes.
 */
uint32_t monitor_fuse_current_max_get(void);

/**
 * @brief Clear fuse alert event.
 *
 */
void monitor_fuse_clear(void);
