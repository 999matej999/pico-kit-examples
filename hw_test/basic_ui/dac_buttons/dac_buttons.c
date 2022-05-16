#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LED1 6
#define LED2 7
#define LED3 8
#define LED4 9

#define DAC_BUTTONS 28        // input gpio for dac buttons
#define DAC_BUTTONS_ADC_CH 2  // input channel of adc

int main()
{
    stdio_init_all();
    printf("ADC Example, measuring GPIO28\n");

    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(DAC_BUTTONS);
    adc_select_input(DAC_BUTTONS_ADC_CH);

    gpio_init(LED1);
    gpio_init(LED2);
    gpio_init(LED3);
    gpio_init(LED4);

    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_set_dir(LED3, GPIO_OUT);
    gpio_set_dir(LED4, GPIO_OUT);

    while (1)
    {
        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / ((1 << 12) - 1);
        uint16_t result = adc_read();
        float voltage = result * conversion_factor;

        /*printf("BIN value: 0b");
        for(size_t i = 0; i < 12; ++i) printf((result & (1 << (12 - i - 1))) ? "1" : "0");
        printf(", HEX value: 0x%03x, voltage: %f V\n", result, voltage);*/

        uint8_t buttons = round(16*voltage/3.3f);

        gpio_put(LED1, buttons & 0b1000);
        gpio_put(LED2, buttons & 0b0100);
        gpio_put(LED3, buttons & 0b0010);
        gpio_put(LED4, buttons & 0b0001);

        printf("\nbuttons: |");
        for(size_t i = 0; i < 4; ++i) 
        {
            if((buttons & (1 << (4 - i - 1)))) printf(" S%u |", i + 1);
            else printf("    |");
        }

        sleep_ms(100);
    }
}
