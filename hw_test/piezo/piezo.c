#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define PIEZO 10

int main() {
    gpio_init(PIEZO);

    gpio_set_dir(PIEZO, GPIO_OUT);

    uint64_t frequency = 800;
    int16_t step = 2;
    bool state = false;

    while (1) {
        uint64_t delay_time = 1000000ul / frequency;

        uint64_t start = time_us_64();
        while((start + 8000ul) > time_us_64())
        {
            gpio_put(PIEZO, state);
            state = !state;
            sleep_us(delay_time);
        }

        frequency += step;

        if(frequency >= 1200) step = -step;
        if(frequency <= 800) step = -step;
    }
}
