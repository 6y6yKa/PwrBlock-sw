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

#include <cmsis_os2.h>

struct __I2C_HandleTypeDef;

typedef struct
{
    struct __I2C_HandleTypeDef* hi2c;
    osMutexId_t                 mutex;
    osSemaphoreId_t             ready_sem;
} I2CBusHandle;

/**
 * @brief Init I2C bus manager.
 *
 */
void i2c_manager_init(void);
