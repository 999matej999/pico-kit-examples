#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define PIEZO 10

#define START_FREQ 800u
#define STOP_FREQ 1200u
#define STEP_FREQ 2u

#define ONE_TONE_TIME 8000ul

int main() {
    gpio_init(PIEZO);

    gpio_set_dir(PIEZO, GPIO_OUT);

    uint64_t frequency = START_FREQ;
    int16_t step = STEP_FREQ;
    bool state = false;

    while (1) {
        uint64_t delay_time = 1000000ul / frequency;

        uint64_t start = time_us_64();
        while((start + ONE_TONE_TIME) > time_us_64())
        {
            gpio_put(PIEZO, state);
            state = !state;
            sleep_us(delay_time);
        }

        frequency += step;

        if(frequency >= STOP_FREQ) step = -step;
        if(frequency <= START_FREQ) step = -step;
    }
}
