#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "SPI.h"
#include "MCP23S018.h"

int main()
{
    SPI spi;

    MCP23S018 expander(spi);

    stdio_init_all();

    printf("SPI expander example\n");

    while(true)
    {
        uint8_t buffer = 0;
        uint8_t value = expander.read();
        expander.setDirection(0xF0);
        expander.write((value & 0xF0) >> 4);

        for (size_t i = 0; i < 8; ++i)
        {
            printf("%c", (buffer & (1 << i)) ? '1' : '0');
        }
        printf("\n");

        sleep_ms(10);
    }

    return 0;
}