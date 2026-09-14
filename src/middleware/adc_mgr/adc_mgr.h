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

#include <stdint.h>

typedef enum
{
    ADC_MNG_CH_VUSB_PWR_ADC = 0,
    ADC_MNG_CH_VUSB_PC_ADC,
    ADC_MNG_CH_VUSB_NTC_IN,

    ADC_MNG_CH_NUM
} AdcManagerChannel_t;

/**
 * @brief Init ADC module manager.
 *
 */
void adc_manager_init(void);

/**
 * @brief Reading voltage from ADC.
 *
 * @param channel Selected channel of ADC.
 * @return float Voltage value in Volts.
 */
float adc_manager_read(uint32_t channel);
