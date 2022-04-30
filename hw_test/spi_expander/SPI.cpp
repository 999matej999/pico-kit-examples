#include "SPI.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

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

SPI::SPI(/* args */)
{
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
}

SPI::~SPI()
{
    spi_deinit(spi0);
}

void SPI::write_register(uint8_t reg, uint8_t data)
{
    uint8_t msg[3] = {};
    msg[0] = deviceOpcode;
    msg[1] = reg;
    msg[2] = data;
    cs_select();
    spi_write_blocking(spi0, msg, 3);
    cs_deselect();
    sleep_ms(10);
}

void SPI::read_registers(uint8_t reg, uint8_t *buf, uint16_t len)
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