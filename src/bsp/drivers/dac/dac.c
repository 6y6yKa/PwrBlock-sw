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

#include "dac.h"

#include <stm32g0xx_hal.h>

#define DAC_VREF     (2.5F)
#define DAC_MAX_CODE (4095U)

extern DAC_HandleTypeDef  hdac1;
static DAC_HandleTypeDef* hdac_drv = &hdac1;

void dac_write(float value)
{
    // Correct rounding with adder 0.5F
    int32_t signed_code =
        (int32_t)((value / DAC_VREF) * (float)DAC_MAX_CODE + 0.5F);

    // Checkout saturation
    if (signed_code > (int32_t)DAC_MAX_CODE) {
        signed_code = DAC_MAX_CODE;
    }
    else if (signed_code < 0) {
        signed_code = 0;
    }

    uint32_t dac_code = (uint32_t)signed_code;

    HAL_DAC_SetValue(hdac_drv, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_code);
}
