#include "SPI.h"

SPI::SPI(spi_inst_t *spi_, uint baudrate, uint rx_pin, uint sck_pin, uint tx_pin) : spi(spi_)
{
    spi_init(spi, baudrate);
    gpio_set_function(rx_pin, GPIO_FUNC_SPI);
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(tx_pin, GPIO_FUNC_SPI);
}

SPI::~SPI()
{
    spi_deinit(spi);
}

void SPI::write(uint cs_pin, const uint8_t* send_buf, size_t send_len)
{
    cs_select(cs_pin);
    spi_write_blocking(spi, send_buf, send_len);
    cs_deselect(cs_pin);
    sleep_ms(10);
}

void SPI::read(uint cs_pin, const uint8_t* send_buf, size_t send_len, uint8_t *recv_buf, size_t recv_len)
{
    cs_select(cs_pin);
    spi_write_blocking(spi, send_buf, send_len);
    sleep_ms(10);
    spi_read_blocking(spi, 0, recv_buf, recv_len);
    cs_deselect(cs_pin);
    sleep_ms(10);
}
