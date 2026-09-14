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

#include "exti.h"

#include "main.h"

#define EXTI_CALLBACKS_MAX (32)

typedef struct
{
    ExtiCallback on_rising;
    ExtiCallback on_falling;
} ExtiEdgeCallbacks;

static ExtiEdgeCallbacks callbacks[EXTI_CALLBACKS_MAX];
static const uint16_t    PINS[EXTI_CONSUMER_NUM] = {
    [EXTI_CONSUMER_BUTTON] = BUTTON_Pin,
    [EXTI_CONSUMER_INA229] = INA_ALERT_Pin,
};

void exti_register_cb(
    ExtiConsumer cons, ExtiCallback on_rising, ExtiCallback on_falling
)
{
    uint8_t pin_idx               = __builtin_ctz(PINS[cons]);
    callbacks[pin_idx].on_rising  = on_rising;
    callbacks[pin_idx].on_falling = on_falling;
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    uint8_t pin_idx = __builtin_ctz(GPIO_Pin);
    if (pin_idx < EXTI_CALLBACKS_MAX && callbacks[pin_idx].on_falling != NULL) {
        callbacks[pin_idx].on_falling();
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    uint8_t pin_idx = __builtin_ctz(GPIO_Pin);
    if (pin_idx < EXTI_CALLBACKS_MAX && callbacks[pin_idx].on_rising != NULL) {
        callbacks[pin_idx].on_rising();
    }
}
