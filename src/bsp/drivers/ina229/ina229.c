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

#include "ina229.h"

#include "ina229_conf.h"
#include "ina229_reg.h"

#include <string.h>

#include "main.h"

#define INA229_SPI SPI1
extern SPI_HandleTypeDef  hspi1;
static SPI_HandleTypeDef* spi = &hspi1;

typedef struct
{
    uint8_t  tx_buf[INA229_FRAME_SIZE_MAX];
    uint8_t  rx_buf[INA229_FRAME_SIZE_MAX];
    uint8_t* rx_dst;
    uint8_t  frame_size;

    float    curr_lsb;
    Callback cb_transfer_cmplt;
} INA229;

// Register sizes in bytes
static const uint8_t INA229_REG_SIZE[INA229_REG_NUM] = {
    [INA229_REG_CONFIG]          = 2,
    [INA229_REG_ADC_CONFIG]      = 2,
    [INA229_REG_SHUNT_CAL]       = 2,
    [INA229_REG_SHUNT_TEMPCO]    = 2,
    [INA229_REG_VSHUNT]          = 3,
    [INA229_REG_VBUS]            = 3,
    [INA229_REG_DIETEMP]         = 2,
    [INA229_REG_CURRENT]         = 3,
    [INA229_REG_POWER]           = 3,
    [INA229_REG_ENERGY]          = 5,
    [INA229_REG_CHARGE]          = 5,
    [INA229_REG_DIAG_ALRT]       = 2,
    [INA229_REG_SOVL]            = 2,
    [INA229_REG_SUVL]            = 2,
    [INA229_REG_BOVL]            = 2,
    [INA229_REG_BUVL]            = 2,
    [INA229_REG_TEMP_LIMIT]      = 2,
    [INA229_REG_PWR_LIMIT]       = 2,
    [INA229_REG_MANUFACTURER_ID] = 2,
    [INA229_REG_DEVICE_ID]       = 2,
};

static INA229 ina;

// Private prototypes
static bool spi_transfer(uint8_t* tx_data, uint8_t* rx_data, uint16_t size);

void ina229_init(float max_curr, Callback cb_transfer_cmplt)
{
    ina.curr_lsb          = max_curr / (1 << 19);
    ina.cb_transfer_cmplt = cb_transfer_cmplt;
}

bool ina229_reg_read(Ina229RegisterName addr, uint8_t* dst)
{
    // Prepare control word
    ina.tx_buf[0] = (addr << 2) | INA229_READ_BIT;
    // Setup total frame size
    ina.frame_size = INA229_REG_SIZE[addr] + 1;
    // Setup rx destination
    ina.rx_dst = dst;
    // Start transaction
    return spi_transfer(ina.tx_buf, ina.rx_buf, ina.frame_size);
}

bool ina229_reg_write(Ina229RegisterName addr, uint8_t* src)
{
    // Prepare control word
    ina.tx_buf[0] = (addr << 2) | INA229_WRITE_BIT;
    // Setup total frame size
    ina.frame_size = INA229_REG_SIZE[addr] + 1;
    // Copy target value of register to tx_buff
    memcpy(&ina.tx_buf[1], src, INA229_REG_SIZE[addr]);
    // Start transaction
    return spi_transfer(ina.tx_buf, ina.rx_buf, ina.frame_size);
}

void ina229_abort(void)
{
    // Up CS line
    HAL_GPIO_WritePin(INA_SPI_NSS_GPIO_Port, INA_SPI_NSS_Pin, GPIO_PIN_SET);
    HAL_SPI_Abort(spi);
}

void ina229_serialize_rst(uint8_t* dst)
{
    uint16_t reg_val = INA229_REG_CONFIG_RST_Msk;

    dst[0] = (reg_val >> 8) & 0xFF;
    dst[1] = reg_val & 0xFF;
}

void ina229_serialize_adc_cfg(Ina229ADCcfg cfg, uint8_t* dst)
{
    uint16_t reg_val = (cfg.mode << INA229_REG_ADC_CONFIG_MODE_Pos) |
                       (cfg.vbusct << INA229_REG_ADC_CONFIG_VBUSCT_Pos) |
                       (cfg.vshct << INA229_REG_ADC_CONFIG_VSHCT_Pos) |
                       (cfg.vtct << INA229_REG_ADC_CONFIG_VTCT_Pos) |
                       (cfg.avg << INA229_REG_ADC_CONFIG_AVG_Pos);

    dst[0] = (reg_val >> 8) & 0xFF;
    dst[1] = reg_val & 0xFF;
}

void ina229_serialize_shunt_cal(float shunt_val, uint8_t* dst)
{
    // Calc by default for ADCRANGE=0, for ADCRANGE=1 multiply shunt_cal by 4
    uint16_t shunt_cal =
        INA229_SHUNT_CAL_CONV_FACTOR * ina.curr_lsb * shunt_val;

    dst[0] = (shunt_cal >> 8) & 0xFF;
    dst[1] = shunt_cal & 0xFF;
}

float ina229_deserialize_vshunt(const uint8_t* buf)
{
    int32_t val =
        (((int32_t)buf[0] << 16) | ((int32_t)buf[1] << 8) | buf[2]) >> 4;

    // Parse two's complement for 20bit value of register
    if (val & (1 << 19)) {
        val -= (1 << 20);
    }

    // TODO: using INA229_VSHUNT_CONV_FACTOR_RANGE_0 by default, usage RANGE_1
    // not implemented yet
    return (float)val * INA229_VSHUNT_CONV_FACTOR_MV_RANGE_0;
}

float ina229_deserialize_vbus(const uint8_t* buf)
{
    int32_t val =
        (((int32_t)buf[0] << 16) | ((int32_t)buf[1] << 8) | buf[2]) >> 4;

    // Parse two's complement for 20bit value of register
    if (val & (1 << 19)) {
        val -= (1 << 20);
    }

    return (float)val * INA229_VBUS_CONV_FACTOR_MV;
}

float ina229_deserialize_dietemp(const uint8_t* buf)
{
    int32_t val = ((int32_t)buf[0] << 8) | buf[1];
    return (float)val * INA229_DIETEMP_CONV_FACTOR_MC;
}

float ina229_deserialize_current(const uint8_t* buf)
{
    int32_t val =
        (((int32_t)buf[0] << 16) | ((int32_t)buf[1] << 8) | buf[2]) >> 4;

    // Parse two's complement for 20bit value of register
    if (val & (1 << 19)) {
        val -= (1 << 20);
    }
    return (float)val * ina.curr_lsb;
}

float ina229_deserialize_power(const uint8_t* buf)
{
    uint32_t val = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];
    return (float)val * ina.curr_lsb * INA229_POWER_CONV_FACTOR;
}

void ina229_serialize_sovl(float shunt_voltage, uint8_t* dst)
{
    // Calc by default for ADCRANGE=0 (5 uV/LSB), for ADCRANGE=1 use 1.25 uV/LSB
    int16_t val = (int16_t)(shunt_voltage / INA229_SOVL_CONV_FACTOR_RANGE_0);

    dst[0] = (val >> 8) & 0xFF;
    dst[1] = val & 0xFF;
}

float ina229_deserialize_sovl(const uint8_t* buf)
{
    int16_t val = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);
    // Calc by default for ADCRANGE=0 (5 uV/LSB), for ADCRANGE=1 use 1.25 uV/LSB
    return (float)val * INA229_SOVL_CONV_FACTOR_RANGE_0;
}

bool ina229_diag_check(const uint8_t* buf, Ina229DiagFlag flag)
{
    uint16_t val = ((uint16_t)buf[0] << 8) | buf[1];
    return val & flag;
}

static bool spi_transfer(uint8_t* tx_data, uint8_t* rx_data, uint16_t size)
{
    // Drop CS line
    HAL_GPIO_WritePin(INA_SPI_NSS_GPIO_Port, INA_SPI_NSS_Pin, GPIO_PIN_RESET);

    if (HAL_SPI_TransmitReceive_IT(spi, tx_data, rx_data, size) != HAL_OK) {
        return false;
    }

    return true;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == INA229_SPI) {
        // Up CS line
        HAL_GPIO_WritePin(INA_SPI_NSS_GPIO_Port, INA_SPI_NSS_Pin, GPIO_PIN_SET);
        memcpy(ina.rx_dst, &ina.rx_buf[1], ina.frame_size - 1);
        ina.cb_transfer_cmplt();
    }
}
