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

#define INA229_FRAME_SIZE_MAX (6)

#define INA229_SHUNT_CAL_CONV_FACTOR (13107.2F * 1000000.0F)

#define INA229_VSHUNT_CONV_FACTOR_MV_RANGE_0 (0.0003125F)
#define INA229_VSHUNT_CONV_FACTOR_MV_RANGE_1 (0.00078125F)

#define INA229_VBUS_CONV_FACTOR_MV (0.1953125F)

#define INA229_DIETEMP_CONV_FACTOR_MC (7.8125F)

#define INA229_POWER_CONV_FACTOR (3.2F)

#define INA229_ENERGY_CONV_FACTOR (16.0F * 3.2F)

#define INA229_SOVL_CONV_FACTOR_RANGE_0 (0.000005F)
#define INA229_SOVL_CONV_FACTOR_RANGE_1 (0.00000125F)
