#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED1 6
#define LED2 7
#define LED3 8
#define LED4 9

int main() {
    gpio_init(LED1);
    gpio_init(LED2);
    gpio_init(LED3);
    gpio_init(LED4);

    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_set_dir(LED3, GPIO_OUT);
    gpio_set_dir(LED4, GPIO_OUT);

    uint8_t counter = 0;

    while (1) {
        gpio_put(LED1, counter & 0b1000);
        gpio_put(LED2, counter & 0b0100);
        gpio_put(LED3, counter & 0b0010);
        gpio_put(LED4, counter & 0b0001);

        ++counter;

        sleep_ms(100);
    }
}
