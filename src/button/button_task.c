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

#include "button_task.h"

#include "bsp/drivers/exti/exti.h"

#include "regulator/regulator_task.h"

#include <cmsis_os2.h>
#include <stdbool.h>

#define BUTTON_PRESS_THRESHOLD (500)

static osThreadId_t button_thread_id;

// Private prototypes
static void push_cb(void);
static void release_cb(void);

void button_app_task(void* argument)
{
    (void)argument;

    button_thread_id = osThreadGetId();
    uint32_t press_time;

    // Register callback for EXTI broker
    exti_register_cb(EXTI_CONSUMER_BUTTON, push_cb, release_cb);

    while (true) {
        // Wait for button press
        osThreadFlagsWait(1 << 0, osFlagsWaitAny, osWaitForever);

        // Get press timestamp
        press_time = osKernelGetTickCount();
        // Debounce
        osDelay(40);
        osThreadFlagsClear(1);

        // Wait for button release
        osThreadFlagsWait(1 << 1, osFlagsWaitAny, osWaitForever);
        // Debounce
        osDelay(40);

        // Get button holding time
        uint32_t duration = osKernelGetTickCount() - press_time;

        if (duration < BUTTON_PRESS_THRESHOLD) {
            regulator_output_toggle();
        }
        else {
            // Long press
        }
    }
}

static void push_cb(void)
{
    osThreadFlagsSet(button_thread_id, 1 << 0);
}

static void release_cb(void)
{
    osThreadFlagsSet(button_thread_id, 1 << 1);
}
