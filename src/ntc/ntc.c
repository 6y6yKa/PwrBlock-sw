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

#include "ntc.h"

#include "middleware/adc_mgr/adc_mgr.h"

#include <math.h>

// Standard Steinhart-Hart Coefficients for a 10k NTC
const float NTC_A    = 0.001129148F;
const float NTC_B    = 0.000234125F;
const float NTC_C    = 0.0000000876741F;
const float NTC_VREF = 2.5F;

// Fixed resistor to GND value
const float NTC_R_BALANCE = 10000.0F;

// Epsilon for voltage tolerance
const float NTC_V_TOLERANCE = 0.0001F;

int32_t ntc_temp_get(void)
{
    float ntc_v = adc_manager_read(ADC_MNG_CH_VUSB_NTC_IN);

    // Prevent division by zero if ADC reads absolute 0
    if (ntc_v <= NTC_V_TOLERANCE) {
        ntc_v = NTC_V_TOLERANCE;
    }

    // Prevent short circuit on NTC
    if (ntc_v >= (NTC_VREF - NTC_V_TOLERANCE)) {
        ntc_v = NTC_VREF - NTC_V_TOLERANCE;
    }

    float ntc_r = NTC_R_BALANCE * ((NTC_VREF / ntc_v) - 1.0F);

    // Apply the Steinhart-Hart Equation
    float ln_r  = logf(ntc_r);
    float inv_t = NTC_A + (NTC_B * ln_r) + (NTC_C * ln_r * ln_r * ln_r);

    // Convert Kelvin to millidegrees Celsius
    return (int32_t)(((1.0F / inv_t) - 273.15F) * 1000.0F);
}
