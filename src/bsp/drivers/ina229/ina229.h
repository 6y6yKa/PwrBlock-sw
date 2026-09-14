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

#include <stdbool.h>
#include <stdint.h>

// Register map
typedef enum
{
    INA229_REG_CONFIG          = 0x00U,
    INA229_REG_ADC_CONFIG      = 0x01U,
    INA229_REG_SHUNT_CAL       = 0x02U,
    INA229_REG_SHUNT_TEMPCO    = 0x03U,
    INA229_REG_VSHUNT          = 0x04U,
    INA229_REG_VBUS            = 0x05U,
    INA229_REG_DIETEMP         = 0x06U,
    INA229_REG_CURRENT         = 0x07U,
    INA229_REG_POWER           = 0x08U,
    INA229_REG_ENERGY          = 0x09U,
    INA229_REG_CHARGE          = 0x0AU,
    INA229_REG_DIAG_ALRT       = 0x0BU,
    INA229_REG_SOVL            = 0x0CU,
    INA229_REG_SUVL            = 0x0DU,
    INA229_REG_BOVL            = 0x0EU,
    INA229_REG_BUVL            = 0x0FU,
    INA229_REG_TEMP_LIMIT      = 0x10U,
    INA229_REG_PWR_LIMIT       = 0x11U,
    INA229_REG_MANUFACTURER_ID = 0x3EU,
    INA229_REG_DEVICE_ID       = 0x3FU,

    INA229_REG_NUM
} Ina229RegisterName;

typedef enum
{
    INA229_ADC_MODE_SHUTDOWN1 = 0,
    INA229_ADC_MODE_SINGLE_VBUS,
    INA229_ADC_MODE_SINGLE_VSHUNT,
    INA229_ADC_MODE_SINGLE_VBUS_VSHUNT,
    INA229_ADC_MODE_SINGLE_DIETEMP,
    INA229_ADC_MODE_SINGLE_VBUS_DIETEMP,
    INA229_ADC_MODE_SINGLE_VSHUNT_DIETEMP,
    INA229_ADC_MODE_SINGLE_VBUS_VSUNT_DIETEMP,
    INA229_ADC_MODE_SHUTDOWN2,
    INA229_ADC_MODE_CONT_VBUS,
    INA229_ADC_MODE_CONT_VSHUNT,
    INA229_ADC_MODE_CONT_VBUS_VSHUNT,
    INA229_ADC_MODE_CONT_DIETEMP,
    INA229_ADC_MODE_CONT_VBUS_DIETEMP,
    INA229_ADC_MODE_CONT_VSHUNT_DIETEMP,
    INA229_ADC_MODE_CONT_VBUS_VSUNT_DIETEMP
} Ina229ADCmode;

typedef enum
{
    INA229_ADC_CONV_T_50_US = 0,
    INA229_ADC_CONV_T_84_US,
    INA229_ADC_CONV_T_150_US,
    INA229_ADC_CONV_T_280_US,
    INA229_ADC_CONV_T_540_US,
    INA229_ADC_CONV_T_1052_US,
    INA229_ADC_CONV_T_2074_US,
    INA229_ADC_CONV_T_4120_US,
} Ina229ADCconvT;

typedef enum
{
    INA229_ADC_AVG_1 = 0,
    INA229_ADC_AVG_4,
    INA229_ADC_AVG_16,
    INA229_ADC_AVG_64,
    INA229_ADC_AVG_128,
    INA229_ADC_AVG_256,
    INA229_ADC_AVG_512,
    INA229_ADC_AVG_1024,
} Ina229ADCavg;

typedef struct
{
    Ina229ADCmode  mode;
    Ina229ADCconvT vbusct;
    Ina229ADCconvT vshct;
    Ina229ADCconvT vtct;
    Ina229ADCavg   avg;
} Ina229ADCcfg;

typedef enum
{
    INA229_DIAG_SHNTOL = 1 << 6,
    INA229_DIAG_TMPOL  = 1 << 7,
    INA229_DIAG_BUSOL  = 1 << 4,
} Ina229DiagFlag;

typedef void (*Callback)(void);

/**
 * @brief Init internal instance of driver.
 *
 * @param max_curr Max current expected via shunt (in amperes).
 * @param cb_transfer_cmplt Callback on transfer complete via SPI. Called after
 * SPI RxTx done.
 */
void ina229_init(float max_curr, Callback cb_transfer_cmplt);

/**
 * @brief Start register read process.
 *
 * @param addr Register address.
 * @param dst Destination buffer.
 * @return true Process started successfully.
 * @return false  Process start fail.
 */
bool ina229_reg_read(Ina229RegisterName addr, uint8_t* dst);

/**
 * @brief Start register write process.
 *
 * @param addr Register address.
 * @param dst Source buffer.
 * @return true Process started successfully.
 * @return false  Process start fail.
 */
bool ina229_reg_write(Ina229RegisterName addr, uint8_t* src);

/**
 * @brief Abort ongoing spi communication.
 *
 */
void ina229_abort(void);

/**
 * @brief Serializer for reset command.
 *
 * @param dst Destination buffer.
 */
void ina229_serialize_rst(uint8_t* dst);

/**
 * @brief Serializer for SHUNT_CAL value.
 *
 * @param val Value to be set in SHUNT_CAL register.
 * @param dst Destination buffer.
 */
void ina229_serialize_shunt_cal(float val, uint8_t* dst);

/**
 * @brief Serializer for ADC_CONFIG value.
 *
 * @param cfg Config structure.
 * @param dst Destination buffer.
 */
void ina229_serialize_adc_cfg(Ina229ADCcfg cfg, uint8_t* dst);

/**
 * @brief Deserializer for VSHUNT register.
 *
 * @param buf Source buffer.
 * @return float Resulting value in millivolts.
 */
float ina229_deserialize_vshunt(const uint8_t* buf);

/**
 * @brief Deserializer for VBUS register.
 *
 * @param buf Source buffer.
 * @return float Resulting value in millivolts.
 */
float ina229_deserialize_vbus(const uint8_t* buf);

/**
 * @brief Deserializer for DIETEMP register.
 *
 * @param buf Source buffer.
 * @return float Resulting value in mC.
 */
float ina229_deserialize_dietemp(const uint8_t* buf);

/**
 * @brief Deserializer for CURRENT register.
 *
 * @param buf Source buffer.
 * @return float Resulting value in amperes.
 */
float ina229_deserialize_current(const uint8_t* buf);

/**
 * @brief Deserializer for POWER register.
 *
 * @param buf Source buffer.
 * @return float Resulting value in watts.
 */
float ina229_deserialize_power(const uint8_t* buf);

/**
 * @brief Serializer for SOVL register.
 *
 * @param shunt_voltage Target shunt overvoltage value in volts.
 * @param dst Destination buffer.
 */
void ina229_serialize_sovl(float shunt_voltage, uint8_t* dst);

/**
 * @brief Deserializer for SOVL register.
 *
 * @param buf Source buffer.
 * @return float Resulting value in volts.
 */
float ina229_deserialize_sovl(const uint8_t* buf);

/**
 * @brief Diagnostic Flags and Alert check.
 *
 * @param buf Source buffer (raw value of register).
 * @param flag Target DIAG_ALRT flag.
 * @return true Flag is set.
 * @return false Flag is clean.
 */
bool ina229_diag_check(const uint8_t* buf, Ina229DiagFlag flag);
