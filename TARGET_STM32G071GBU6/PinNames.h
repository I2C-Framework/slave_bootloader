/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 *
 * Automatically generated from STM32CubeMX/db/mcu/STM32G071R(6-8-B)Tx.xml
 */

/* MBED TARGET LIST: NUCLEO_G071RB */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
} ALTx;

typedef enum {
    PA_0       = 0x00,
    PA_1       = 0x01,
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_4       = 0x04,
    PA_5       = 0x05,
    PA_6       = 0x06,
    PA_6_ALT0  = PA_6  | ALT0, // same pin used for alternate HW
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_7_ALT1  = PA_7  | ALT1, // same pin used for alternate HW
    PA_7_ALT2  = PA_7  | ALT2, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_15      = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0, // same pin used for alternate HW
    PB_0       = 0x10,
    PB_0_ALT0  = PB_0  | ALT0, // same pin used for alternate HW
    PB_1       = 0x11,
    PB_1_ALT0  = PB_1  | ALT0, // same pin used for alternate HW
    PB_1_ALT1  = PB_1  | ALT1, // same pin used for alternate HW
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_4       = 0x14,
    PB_5       = 0x15,
    PB_6       = 0x16,
    PB_6_ALT0  = PB_6  | ALT0, // same pin used for alternate HW
    PB_7       = 0x17,
    PB_8       = 0x18,
    PB_9       = 0x19,
    PB_10      = 0x1A,
    PB_11      = 0x1B,
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0, // same pin used for alternate HW
    PB_14      = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0, // same pin used for alternate HW
    PB_15      = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0, // same pin used for alternate HW
    PB_15_ALT1 = PB_15 | ALT1, // same pin used for alternate HW
    PC_0       = 0x20,
    PC_1       = 0x21,
    PC_2       = 0x22,
    PC_3       = 0x23,
    PC_4       = 0x24,
    PC_4_ALT0  = PC_4  | ALT0, // same pin used for alternate HW
    PC_5       = 0x25,
    PC_5_ALT0  = PC_5  | ALT0, // same pin used for alternate HW
    PC_6       = 0x26,
    PC_7       = 0x27,
    PC_8       = 0x28,
    PC_8_ALT0  = PC_8  | ALT0, // same pin used for alternate HW
    PC_9       = 0x29,
    PC_9_ALT0  = PC_9  | ALT0, // same pin used for alternate HW
    PC_10      = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0, // same pin used for alternate HW
    PC_11      = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0, // same pin used for alternate HW
    PC_12      = 0x2C,
    PC_13      = 0x2D,
    PC_14      = 0x2E,
    PC_15      = 0x2F,
    PD_0       = 0x30,
    PD_1       = 0x31,
    PD_2       = 0x32,
    PD_3       = 0x33,
    PD_4       = 0x34,
    PD_5       = 0x35,
    PD_6       = 0x36,
    PD_8       = 0x38,
    PD_9       = 0x39,
    PF_0       = 0x50,
    PF_1       = 0x51,
    PF_2       = 0x52,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

    /**** Custom board pin names ****/

    I2C1_SCL = PB_6,
    I2C1_SDA = PB_7,
    I2C2_SCL = PA_9,
    I2C2_SDA = PA_10,

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Uno (Rev3) pins
    ARDUINO_UNO_A0  = PA_0,
    ARDUINO_UNO_A1  = PA_1,
    ARDUINO_UNO_A2  = PA_4,
    ARDUINO_UNO_A3  = PB_1,
    ARDUINO_UNO_A4  = PB_11,
    ARDUINO_UNO_A5  = PB_12,

    ARDUINO_UNO_D0  = PC_5,
    ARDUINO_UNO_D1  = PC_4,
    ARDUINO_UNO_D2  = PA_10,
    ARDUINO_UNO_D3  = PB_3,
    ARDUINO_UNO_D4  = PB_5,
    ARDUINO_UNO_D5  = PB_4,
    ARDUINO_UNO_D6  = PB_14,
    ARDUINO_UNO_D7  = PA_8,
    ARDUINO_UNO_D8  = PA_9,
    ARDUINO_UNO_D9  = PC_7,
    ARDUINO_UNO_D10 = PB_0,
    ARDUINO_UNO_D11 = PA_7,
    ARDUINO_UNO_D12 = PA_6,
    ARDUINO_UNO_D13 = PA_5,
    ARDUINO_UNO_D14 = PB_9,
    ARDUINO_UNO_D15 = PB_8,
#endif

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    CONSOLE_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX = PA_2,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    CONSOLE_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX = PA_3,
#endif

    /**** OSCILLATOR pins ****/
    RCC_OSC32_EN = PC_15,
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,
    RCC_OSC_EN = PC_15,
    RCC_OSC_EN_ALT0 = PF_1,
    RCC_OSC_IN = PF_0,
    RCC_OSC_OUT = PF_1,

    /**** DEBUG pins ****/
    SYS_PVD_IN = PB_7,
    SYS_SWCLK = PA_14,
    SYS_SWDIO = PA_13,
    SYS_WKUP1 = PA_0,
    SYS_WKUP2 = PC_13,
    SYS_WKUP4 = PA_2,
    SYS_WKUP5 = PC_5,
    SYS_WKUP6 = PB_5,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and I2C pinss
#define LED_STATUS          PA_7
#define LED_SENSOR          PB_5
#define I2C_SENSOR_SDA      I2C2_SDA
#define I2C_SENSOR_SCL      I2C2_SCL
#define I2C_FRAMEWORK_SDA   I2C1_SDA
#define I2C_FRAMEWORK_SCL   I2C1_SCL


#ifdef __cplusplus
}
#endif

#endif
