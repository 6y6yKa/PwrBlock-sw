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

// Access flags
#define INA229_WRITE_BIT (0U << 0)
#define INA229_READ_BIT  (1U << 0)

// INA229_REG_CONFIG
#define INA229_REG_CONFIG_RST_Pos (15U)
#define INA229_REG_CONFIG_RST_Msk (0x01U << INA229_REG_CONFIG_RST_Pos)

#define INA229_REG_CONFIG_RSTACC_Pos (14U)
#define INA229_REG_CONFIG_RSTACC_Msk (0x01U << INA229_REG_CONFIG_RSTACC_Pos)

#define INA229_REG_CONFIG_CONVDLY_Pos (6U)
#define INA229_REG_CONFIG_CONVDLY_Msk (0xFFU << INA229_REG_CONFIG_CONVDLY_Pos)

#define INA229_REG_CONFIG_TEMPCOMP_Pos (5U)
#define INA229_REG_CONFIG_TEMPCOMP_Msk (0x01U << INA229_REG_CONFIG_TEMPCOMP_Pos)

#define INA229_REG_CONFIG_ADCRANGE_Pos (4U)
#define INA229_REG_CONFIG_ADCRANGE_Msk (0x01U << INA229_REG_CONFIG_ADCRANGE_Pos)

// INA229_REG_ADC_CONFIG
#define INA229_REG_ADC_CONFIG_MODE_Pos (12U)
#define INA229_REG_ADC_CONFIG_MODE_Msk (0x0FU << INA229_REG_ADC_CONFIG_MODE_Pos)

#define INA229_REG_ADC_CONFIG_VBUSCT_Pos (9U)
#define INA229_REG_ADC_CONFIG_VBUSCT_Msk                                       \
    (0x07U << INA229_REG_ADC_CONFIG_VBUSCT_Pos)

#define INA229_REG_ADC_CONFIG_VSHCT_Pos (6U)
#define INA229_REG_ADC_CONFIG_VSHCT_Msk                                        \
    (0x07U << INA229_REG_ADC_CONFIG_VSHCT_Pos)

#define INA229_REG_ADC_CONFIG_VTCT_Pos (3U)
#define INA229_REG_ADC_CONFIG_VTCT_Msk (0x07U << INA229_REG_ADC_CONFIG_VTCT_Pos)

#define INA229_REG_ADC_CONFIG_AVG_Pos (0U)
#define INA229_REG_ADC_CONFIG_AVG_Msk (0x07U << INA229_REG_ADC_CONFIG_AVG_Pos)
