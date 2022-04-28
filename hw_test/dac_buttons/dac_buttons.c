/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int main() {
    stdio_init_all();
    printf("ADC Example, measuring GPIO28\n");

    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(28);
    // Select ADC input 2 (GPIO28)
    adc_select_input(2);

    gpio_init(2);
    gpio_init(3);
    gpio_init(4);
    gpio_init(5);

    gpio_set_dir(2, GPIO_OUT);
    gpio_set_dir(3, GPIO_OUT);
    gpio_set_dir(4, GPIO_OUT);
    gpio_set_dir(5, GPIO_OUT);

    uint8_t counter = 0;

    while (1) {
        gpio_put(2, counter & 0b0001);
        gpio_put(3, counter & 0b0010);
        gpio_put(4, counter & 0b0100);
        gpio_put(5, counter & 0b1000);

        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        /*printf("Counter: %u, ", counter & 0xf);
        printf("BIN value: 0b");
        for(size_t i = 0; i < 12; ++i) printf((result & (1 << (12 - i - 1))) ? "1" : "0");
        printf(", HEX value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);*/

        uint8_t buttons = round(4.8562 * result * conversion_factor - 0.0456);

        /*printf("pressed buttons: ");
        for(size_t i = 0; i < 4; ++i) printf((buttons & (1 << (4 - i - 1))) ? "1" : "0");
        printf("\n");*/

        printf("\nbuttons: |");
        for(size_t i = 0; i < 4; ++i) 
        {
            if((buttons & (1 << (4 - i - 1)))) printf(" S%u |", i + 1);
            else printf("    |");
        }

        ++counter;
        sleep_ms(100);
    }
}
