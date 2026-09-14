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

#include "24aa02_def.h"

#include <stdbool.h>
#include <stdint.h>

bool ee24aa025_read(
    void* bus_handler, uint8_t page_addr, uint8_t* rx_buff, uint16_t len
);
bool ee24aa025_write(
    void* bus_handler, uint8_t page_addr, uint8_t* rx_buff, uint16_t len
);
