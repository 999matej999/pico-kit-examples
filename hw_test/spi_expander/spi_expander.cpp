#include <stdio.h>
#include "pico/stdlib.h"
#include "SPI.h"
#include "MCP23S018.h"

int main()
{
    stdio_init_all();
    printf("SPI expander example\n");

    SPI spi;

    MCP23S018 expander(spi);

    expander.setDirection(0xF0);

    while(true)
    {
        uint8_t value = expander.read();

        for (int8_t i = 7; i >= 0; --i)
        {
            printf("%c", (value & (1 << i)) ? '1' : '0');
        }
        printf("\n");
        
        expander.write((value & 0xF0) >> 4);

        sleep_ms(10);
    }

    return 0;
}