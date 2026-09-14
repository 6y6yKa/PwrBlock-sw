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

#include "24aa02.h"
#include "24aa02_conf.h"

#include <stm32g0xx_hal.h>

bool ee24aa025_read(
    void* bus_handler, uint8_t page_addr, uint8_t* rx_buff, uint16_t len
)
{
    return HAL_I2C_Mem_Read_IT(
               (I2C_HandleTypeDef*)bus_handler,
               EE24AA025_I2C_ADDR,
               page_addr,
               I2C_MEMADD_SIZE_8BIT,
               rx_buff,
               len
           ) == HAL_OK;
}

bool ee24aa025_write(
    void* bus_handler, uint8_t page_addr, uint8_t* rx_buff, uint16_t len
)
{
    return HAL_I2C_Mem_Write_IT(
               (I2C_HandleTypeDef*)bus_handler,
               EE24AA025_I2C_ADDR,
               page_addr,
               I2C_MEMADD_SIZE_8BIT,
               rx_buff,
               len
           ) == HAL_OK;
}
