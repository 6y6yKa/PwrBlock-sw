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

// Config HW connection to MCU periph
#define WS2812B_TIMER    htim4
#define WS2812B_TIMER_CH TIM_CHANNEL_2

// Config length of led strip
#define WS2812B_NUM (4)

// Config for data buffer
#define WS2812B_DATA_SIZE   (WS2812B_NUM * 24)
#define WS2812B_RESET_SLOTS (250)
#define WS2812B_BUFF_SIZE   (WS2812B_RESET_SLOTS + WS2812B_DATA_SIZE)

// Config for bit timings
#define WS2812B_LED_BIT_SET   (53)  // send 1 --- 2/3 of period of TIM
#define WS2812B_LED_BIT_RESET (26)  // send 0 --- 1/3 of period of TIM
