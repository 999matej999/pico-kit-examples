#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/float.h"
#include "pico/multicore.h"

#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/pio.h"

#include "SPI.h"
#include "MCP23S018.h"
#include "ssd1306.h"

#include "quadrature_encoder.pio.h"

#define SLEEPTIME 25

// PIEZO
#define PIEZO 10
#define START_FREQ 1600u
#define STOP_FREQ 2200u
#define STEP_FREQ 2u
#define ONE_TONE_TIME 3000ul

//#define SIREN_UP_DOWN

// LEDs
#define LED3 8ul
#define LED4 9ul

// ENC
#define ENC_BUTTON 11

// SPI
#define SPI_RX_PIN  4
#define SPI_SCK_PIN 6
#define SPI_TX_PIN  7
#define EXPANDER_CS_PIN 26
#define MCP23S018_DEVICE_OPCODE 0b01000000

// POTENTIOMETER
#define POTENTIOMETER 27        // input gpio for potentiometer
#define POTENTIOMETER_ADC_CH 1  // input channel of adc

// DAC BUTTONS
#define DAC_BUTTONS 28        // input gpio for dac buttons
#define DAC_BUTTONS_ADC_CH 2  // input channel of adc

void display();

void core1_entry()
{
    // piezo
    gpio_init(PIEZO);
    gpio_set_dir(PIEZO, GPIO_OUT);

    // enc button
    gpio_init(ENC_BUTTON);
    gpio_set_dir(ENC_BUTTON, GPIO_IN);

    uint64_t frequency = START_FREQ;
    int16_t step = STEP_FREQ;
    bool state = false;

    while (1)
    {
        uint64_t delay_time = 1000000ul / frequency;

        uint64_t start = time_us_64();
        while((start + ONE_TONE_TIME) > time_us_64())
        {
            bool enc_button = gpio_get(ENC_BUTTON);
            if (enc_button)
            {
                gpio_put(PIEZO, state);
                state = !state;
                sleep_us(delay_time);
            }
        }

        frequency += step;

        #ifdef SIREN_UP_DOWN
        if(frequency >= STOP_FREQ) step = -step;
        if(frequency <= START_FREQ) step = -step;
        #else
        if(frequency >= STOP_FREQ) frequency = START_FREQ;
        #endif
    }
}

int main()
{
    stdio_init_all();

    multicore_launch_core1(core1_entry);

    // leds
    gpio_init(LED3);
    gpio_set_dir(LED3, GPIO_OUT);
    gpio_init(LED4);
    gpio_set_dir(LED4, GPIO_OUT);

    // encoder
    int new_value, delta, old_value = 0;
    int16_t enc_val = 0;
    int16_t enc_val_ = 0;
    const uint PIN_AB = 21;
    PIO pio = pio0;
    const uint sm = 0;
    uint offset = pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, sm, offset, PIN_AB, 0);
    
    // expander
    SPI spi(spi0, 500 * 1000, SPI_RX_PIN, SPI_SCK_PIN, SPI_TX_PIN);
    MCP23S018 expander(spi, EXPANDER_CS_PIN, MCP23S018_DEVICE_OPCODE);
    expander.setDirection(0xF0);

    // oled
    i2c_init(i2c1, 400000);
    gpio_set_function(2, GPIO_FUNC_I2C);
    gpio_set_function(3, GPIO_FUNC_I2C);

    ssd1306_t disp;
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&disp);

    // adc
    adc_init();
    adc_gpio_init(DAC_BUTTONS);
    adc_gpio_init(POTENTIOMETER);  


    while(true)
    {
        // expander
        uint8_t value = expander.read();
        value = value >> 4;
        value = ((value & 0x08) >> 3) | ((value & 0x04) >> 1) | ((value & 0x02) << 1) | ((value & 0x01) << 3);
        //value = value << 4;


        // dac buttons
        adc_select_input(DAC_BUTTONS_ADC_CH);
        const float conversion_factor = 3.3f / ((1 << 12) - 1);
        uint16_t result = adc_read();
        float voltage = result * conversion_factor;
        uint8_t buttons = round(16*voltage/3.3f);
        //buttons = ((buttons & 0x08) >> 3) | ((buttons & 0x04) >> 1) | ((buttons & 0x02) << 1) | ((buttons & 0x01) << 3);
        buttons = buttons << 4;
        buttons |= value & 0x0F;

        for (int8_t i = 7; i >= 0; --i)
        {
            printf("%c", (buttons & (1 << i)) ? '1' : '0');
        }
        printf("\n");


        // encoder
        new_value = quadrature_encoder_get_count(pio, sm);
        delta = new_value - old_value;
        old_value = new_value;
        printf("position %8d, delta %6d\n", new_value, delta);
        enc_val += delta;
        if(enc_val >= (1 << 8))
        {
            enc_val = (1 << 8) - 1;
        }
        else if(enc_val <= 0)
        {
            enc_val = 0;
        }
        enc_val_ = enc_val / 4;

        // LEDs
        expander.write(((enc_val_ & 0x01) << 3) | ((enc_val_ & 0x02) << 1) | ((enc_val_ & 0x04) >> 1) | ((enc_val_ & 0x08) >> 3));
        gpio_put(LED4, enc_val_ & 0x10);
        gpio_put(LED3, enc_val_ & 0x20);


        // potentiometer
        adc_select_input(POTENTIOMETER_ADC_CH);
        uint16_t pot_value = adc_read();
        uint8_t percentage = round(pot_value / 40.95f);
        ssd1306_contrast(&disp, (pot_value >> 4) == 0 ? 1 : (pot_value >> 4));


        // display
        ssd1306_clear(&disp);
        char buf[20] = {};
        sprintf(buf, "RPiPicoKit");
        ssd1306_draw_string(&disp, 0, 0, 1, buf);

        sprintf(buf, "%3d%%", percentage);
        ssd1306_draw_string(&disp, 95, 0, 1, buf);

        const uint32_t width = 10;
        const uint32_t height = 10;

        sprintf(buf, "LEDs:      %3d", enc_val_);
        ssd1306_draw_string(&disp, 0, 16, 1, buf);

        for (int i = 0; i < 6; ++i)
        {
            if(enc_val_ & (1 << (5 - i)))
            ssd1306_draw_square(&disp, 40 + (width + 2) * i, 27, width + 1, height + 1);
            else
            ssd13606_draw_empty_square(&disp, 40 + (width + 2) * i, 27, width, height);
        }

        sprintf(buf, "Buttons:   %3d", buttons);
        ssd1306_draw_string(&disp, 0, 42, 1, buf);

        for (int i = 0; i < 8; ++i)
        {
            if(buttons & (1 << (7 - i)))
            ssd1306_draw_square(&disp, 16 + (width + 2) * i, 53, width + 1, height + 1);
            else
            ssd13606_draw_empty_square(&disp, 16 + (width + 2) * i, 53, width, height);
        }
        ssd1306_show(&disp);


        // delay
        sleep_ms(10);
    }

    return 0;
}

void display()
{

    #if 0
    char *words[]= {"SSD1306", "DISPLAY", "DRIVER"};

    ssd1306_t disp;
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&disp);

    printf("ANIMATION!\n");


    for(int y=0; y<31; ++y) {
        ssd1306_draw_line(&disp, 0, y, 127, y);
        ssd1306_show(&disp);
        sleep_ms(SLEEPTIME);
        ssd1306_clear(&disp);
    }

    for(int y=0, i=1; y>=0; y+=i) {
        ssd1306_draw_line(&disp, 0, 31-y, 127, 31+y);
        ssd1306_draw_line(&disp, 0, 31+y, 127, 31-y);
        ssd1306_show(&disp);
        sleep_ms(SLEEPTIME);
        ssd1306_clear(&disp);
        if(y==32) i=-1;
    }

    for(int i=0; i<sizeof(words)/sizeof(char *); ++i) {
        ssd1306_draw_string(&disp, 8, 24, 2, words[i]);
        ssd1306_show(&disp);
        sleep_ms(800);
        ssd1306_clear(&disp);
    }

    for(int y=31; y<63; ++y) {
        ssd1306_draw_line(&disp, 0, y, 127, y);
        ssd1306_show(&disp);
        sleep_ms(SLEEPTIME);
        ssd1306_clear(&disp);
    }

    #endif
}
