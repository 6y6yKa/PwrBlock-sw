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

typedef enum
{
    EXTI_CONSUMER_BUTTON,
    EXTI_CONSUMER_INA229,

    EXTI_CONSUMER_NUM
} ExtiConsumer;

typedef void (*ExtiCallback)(void);

/**
 * @brief Register callback function for EXTI broker.
 *
 * @param cons Cunsumer of EXTI.
 * @param on_rising Callback function on rising edge.
 * @param on_falling Callback function on falling edge.
 */
void exti_register_cb(
    ExtiConsumer cons, ExtiCallback on_rising, ExtiCallback on_falling
);
