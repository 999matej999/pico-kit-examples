/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

#define ADDR 0x50

// write
//i2c_write_blocking(i2c1, addr, buf, 2, false);

// read
//i2c_write_blocking(i2c1, addr, &val, 1, true); // true to keep master control of bus
//i2c_read_blocking(i2c1, addr, buffer, 6, false); // false - finished with bus

int main() {
    stdio_init_all();

    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    uint8_t buffer[128] = {};

    printf("Reading from address 0x%02X\n", ADDR);

    uint8_t val = 0x00; // writing 0x00 then reading 128 bytes gives to EDID
    //uint8_t val = 0x80; // writing 0x80 then reading 128 bytes gives to Extended data
    i2c_write_blocking(i2c1, ADDR, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c1, ADDR, buffer, 128, false); // false - finished with bus

    printf("   ");
    for (int j = 0; j < 16; ++j)
    {
        printf(" %X ", j);
    }
    printf("\n");
    for (int i = 0; i < 8; ++i)
    {
        printf("%X0", i);
        for (int j = 0; j < 16; ++j)
        {
            printf(" %02X", buffer[i * 16 + j]);
        }
        printf("\n");
    }

    printf("\n");

    val = 0x80; // writing 0x80 then reading 128 bytes gives to Extended data
    i2c_write_blocking(i2c1, ADDR, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c1, ADDR, buffer, 128, false); // false - finished with bus

    printf("   ");
    for (int j = 0; j < 16; ++j)
    {
        printf(" %X ", j);
    }
    printf("\n");
    for (int i = 0; i < 8; ++i)
    {
        printf("%X0", i);
        for (int j = 0; j < 16; ++j)
        {
            printf(" %02X", buffer[i * 16 + j]);
        }
        printf("\n");
    }

    while (1)
    {
        sleep_ms(100);
    }
}
