#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "MCP23S018.h"

#define SPI_RX_PIN  4
#define SPI_SCK_PIN 6
#define SPI_TX_PIN  7
#define SPI_CSN_PIN 26

MCP23S018 expander;

static inline void cs_select()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

static void write_register(uint8_t reg, uint8_t data)
{
    uint8_t buf[2];
    buf[0] = reg & 0x7f;  // remove read bit as this is a write
    buf[1] = data;
    cs_select();
    spi_write_blocking(spi0, buf, 2);
    cs_deselect();
    sleep_ms(10);
}

const uint8_t deviceOpcode = 0b01000000;

static void read_registers(uint8_t reg, uint8_t *buf, uint16_t len)
{
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t msg[2] = {};
    msg[0] = deviceOpcode | 1u;
    msg[1] = reg;
    cs_select();
    spi_write_blocking(spi0, msg, 2);
    sleep_ms(10);
    spi_read_blocking(spi0, 0, buf, len);
    cs_deselect();
    sleep_ms(10);
}

int main()
{
    stdio_init_all();

    printf("SPI expander example\n");

    // This example will use SPI0 at 0.5MHz.
    spi_init(spi0, 500 * 1000);
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    //bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(SPI_CSN_PIN);
    gpio_set_dir(SPI_CSN_PIN, GPIO_OUT);
    gpio_put(SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool
    //bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));

    while(true)
    {
        uint8_t buffer = 0;
        read_registers(GPIO, &buffer, 1);

        for (size_t i = 0; i < 8; ++i)
        {
            printf("%c", (buffer & (1 << i)) ? '1' : '0');
        }
        printf("\n");

        sleep_ms(100);
    }

    return 0;
}