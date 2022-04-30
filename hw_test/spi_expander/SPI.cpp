#include "SPI.h"
#include "hardware/gpio.h"

static inline void cs_select(uint cs_pin)
{
    asm volatile("nop \n nop \n nop");
    gpio_put(cs_pin, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect(uint cs_pin)
{
    asm volatile("nop \n nop \n nop");
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop");
}

SPI::SPI(spi_inst_t *spi_, uint baudrate, uint rx_pin, uint sck_pin, uint tx_pin) : spi(spi_)
{
    spi_init(spi, baudrate);
    gpio_set_function(rx_pin, GPIO_FUNC_SPI);
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(tx_pin, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    //bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));
}

SPI::~SPI()
{
    spi_deinit(spi);
}

void SPI::write_register(uint cs_pin, const uint8_t* send_buf, size_t send_len)
{
    cs_select(cs_pin);
    spi_write_blocking(spi, send_buf, send_len);
    cs_deselect(cs_pin);
    sleep_ms(10);
}

void SPI::read_registers(uint cs_pin, const uint8_t* send_buf, size_t send_len, uint8_t *recv_buf, size_t recv_len)
{
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.
    cs_select(cs_pin);
    spi_write_blocking(spi, send_buf, send_len);
    sleep_ms(10);
    spi_read_blocking(spi, 0, recv_buf, recv_len);
    cs_deselect(cs_pin);
    sleep_ms(10);
}
