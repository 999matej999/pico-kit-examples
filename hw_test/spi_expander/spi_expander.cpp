#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "SPI.h"
#include "MCP23S018.h"

#define SPI_RX_PIN  4
#define SPI_SCK_PIN 6
#define SPI_TX_PIN  7

#define EXPANDER_CS_PIN 26

#define MCP23S018_DEVICE_OPCODE 0b01000000

int main()
{
    stdio_init_all();
    printf("SPI expander example\n");
    
    // This example will use SPI0 at 0.5MHz.
    SPI spi(spi0, 500 * 1000, SPI_RX_PIN, SPI_SCK_PIN, SPI_TX_PIN);

    MCP23S018 expander(spi, EXPANDER_CS_PIN, MCP23S018_DEVICE_OPCODE);

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