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

#include "adc_mgr.h"

#include <cmsis_os2.h>
#include <stm32g0xx_hal.h>

#define ADC_CONVERSION_DELAY_MS (50)

#define ADC_VREF     (2.5F)
#define ADC_MAX_CODE (4095U)
#define ADC_LSB      (ADC_VREF / ADC_MAX_CODE)

const uint32_t ADC_CH[ADC_MNG_CH_NUM] = {
    [ADC_MNG_CH_VUSB_PWR_ADC] = ADC_CHANNEL_0,
    [ADC_MNG_CH_VUSB_PC_ADC]  = ADC_CHANNEL_1,
    [ADC_MNG_CH_VUSB_NTC_IN]  = ADC_CHANNEL_2,
};

typedef struct
{
    osMutexId_t       mutex;
    osSemaphoreId_t   ready_sem;
    volatile uint32_t raw_value;
} ADCModuleHandle;

extern ADC_HandleTypeDef hadc1;

static ADCModuleHandle adc;

void adc_manager_init(void)
{
    const osMutexAttr_t MUTEX_ATTR = {
        "adcMutex", osMutexRecursive | osMutexPrioInherit, NULL, 0
    };
    adc.mutex     = osMutexNew(&MUTEX_ATTR);
    adc.ready_sem = osSemaphoreNew(1, 0, NULL);
}

float adc_manager_read(uint32_t channel)
{
    float result = 0;

    if (osMutexAcquire(adc.mutex, osWaitForever) == osOK) {

        while (osSemaphoreAcquire(adc.ready_sem, 0) == osOK) {
            ;
        }

        ADC_ChannelConfTypeDef cfg = {0};
        cfg.Channel                = ADC_CH[channel];
        cfg.Rank                   = ADC_REGULAR_RANK_1;
        cfg.SamplingTime           = ADC_SAMPLINGTIME_COMMON_1;
        HAL_ADC_ConfigChannel(&hadc1, &cfg);
        HAL_ADC_Start_IT(&hadc1);

        if (osSemaphoreAcquire(adc.ready_sem, ADC_CONVERSION_DELAY_MS) ==
            osOK) {

            result = (float)adc.raw_value * ADC_LSB;
        }
        else {
            HAL_ADC_Stop_IT(&hadc1);
            result = 0.0F;
        }

        osMutexRelease(adc.mutex);
    }

    return result;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        adc.raw_value = HAL_ADC_GetValue(hadc);
        osSemaphoreRelease(adc.ready_sem);
    }
}
