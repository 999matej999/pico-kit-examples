#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "SPI.h"
#include "MCP23S018.h"

int main()
{
    SPI spi;

    MCP23S018 expander;

    stdio_init_all();

    printf("SPI expander example\n");

    while(true)
    {
        uint8_t buffer = 0;
        spi.read_registers(static_cast<uint8_t>(MCP23S018::REG::GPIO), &buffer, 1);
        spi.write_register(static_cast<uint8_t>(MCP23S018::REG::IODIR), 0xF0);
        spi.write_register(static_cast<uint8_t>(MCP23S018::REG::OLAT), (buffer & 0xF0) >> 4);

        for (size_t i = 0; i < 8; ++i)
        {
            printf("%c", (buffer & (1 << i)) ? '1' : '0');
        }
        printf("\n");

        sleep_ms(10);
    }

    return 0;
}