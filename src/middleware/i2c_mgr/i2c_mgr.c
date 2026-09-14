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

#include "i2c_mgr.h"

#include <stm32g0xx_hal.h>

extern I2C_HandleTypeDef hi2c1;

I2CBusHandle i2c_bus1;

void i2c_manager_init(void)
{
    i2c_bus1.hi2c      = &hi2c1;
    i2c_bus1.mutex     = osMutexNew(NULL);
    i2c_bus1.ready_sem = osSemaphoreNew(1, 0, NULL);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == I2C1) {
        osSemaphoreRelease(i2c_bus1.ready_sem);
    }
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == I2C1) {
        osSemaphoreRelease(i2c_bus1.ready_sem);
    }
}
