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

#include "storage.h"

#include "bsp/drivers/24aa02/24aa02.h"
#include "middleware/i2c_mgr/i2c_mgr.h"


#define STORAGE_PAGE_SIZE (EE24AA025E_PAGE_SIZE)
#define STORAGE_EUI       (EE24AAXXX_EUI48_REG)
#define STORAGE_EUI_SIZE  (EE24AAXXX_EUI48_REG_SIZE)

// I2C timeout on read/write operations
#define STORAGE_RXTX_TIMEOUT_MS (50)

// Delay on EEPROM writing process
#define STORAGE_WRITE_DELAY_MS (50)

extern I2CBusHandle i2c_bus1;

osStatus_t storage_mac_get(uint8_t* dst)
{
    return storage_read(STORAGE_EUI, dst, STORAGE_EUI_SIZE);
}

osStatus_t storage_read(uint8_t addr, uint8_t* dst, uint16_t size)
{
    osStatus_t status = osError;

    // Safe I2C bus usage
    if (osMutexAcquire(i2c_bus1.mutex, osWaitForever) != osOK) {
        return osErrorResource;
    }

    do {
        if (ee24aa025_read(i2c_bus1.hi2c, addr, dst, size) == false) {
            status = osError;
            break;
        }

        if (osSemaphoreAcquire(i2c_bus1.ready_sem, STORAGE_RXTX_TIMEOUT_MS) !=
            osOK) {
            status = osError;
            break;
        }

        status = osOK;
    } while (0);

    osMutexRelease(i2c_bus1.mutex);
    return status;
}

osStatus_t storage_write(uint8_t addr, uint8_t* src, uint16_t size)
{
    osStatus_t status          = osError;
    uint16_t   bytes_remaining = size;
    uint8_t*   current_src_ptr = src;
    uint8_t    current_addr    = addr;


    while (bytes_remaining > 0) {
        uint8_t max_write =
            STORAGE_PAGE_SIZE - (current_addr % STORAGE_PAGE_SIZE);
        uint8_t bytes_to_copy =
            (bytes_remaining > max_write) ? max_write : bytes_remaining;

        // Safe I2C bus usage
        if (osMutexAcquire(i2c_bus1.mutex, osWaitForever) != osOK) {
            return osErrorResource;
        }

        // Process writing data
        do {
            if (ee24aa025_write(
                    i2c_bus1.hi2c, current_addr, current_src_ptr, bytes_to_copy
                ) == false) {
                status = osError;
                break;
            }

            if (osSemaphoreAcquire(
                    i2c_bus1.ready_sem, STORAGE_RXTX_TIMEOUT_MS
                ) != osOK) {
                status = osError;
                break;
            }

            status = osOK;
        } while (0);

        // Release mutex in any case of write process
        osMutexRelease(i2c_bus1.mutex);

        if (status != osOK) {
            break;
        }

        // Wait for memory write only if transaction is success
        osDelay(STORAGE_WRITE_DELAY_MS);

        // Update pointers
        bytes_remaining -= bytes_to_copy;
        current_src_ptr += bytes_to_copy;
        current_addr += bytes_to_copy;
    }

    return status;
}
