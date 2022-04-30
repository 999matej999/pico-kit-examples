#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "hardware/spi.h"
#include "hardware/gpio.h"

class SPI
{

private:
    spi_inst_t *spi;
    inline void cs_select(uint cs_pin);
    inline void cs_deselect(uint cs_pin);

public:
    SPI(spi_inst_t *spi_, uint baudrate, uint rx_pin, uint sck_pin, uint tx_pin);
    ~SPI();
    void write(uint cs_pin, const uint8_t* send_buf, size_t send_len);
    void read(uint cs_pin, const uint8_t* send_buf, size_t send_len, uint8_t *recv_buf, size_t recv_len);

};

inline void SPI::cs_select(uint cs_pin)
{
    asm volatile("nop \n nop \n nop");
    gpio_put(cs_pin, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

inline void SPI::cs_deselect(uint cs_pin)
{
    asm volatile("nop \n nop \n nop");
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop");
}

#endif /* __SPI_H__ */
