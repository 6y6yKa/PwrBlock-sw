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

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Ws2812Color;

/**
 * @brief Init WS2812b driver.
 *
 */
void ws2812b_init(void);

/**
 * @brief Set color of single WS2812b.
 *
 * @param pos Address of WS2812b in a row.
 * @param color Color to set.
 */
void ws2812b_set_led(uint32_t pos, Ws2812Color* color);
