#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "hardware/spi.h"

class SPI
{

private:
    const uint8_t deviceOpcode = 0b01000000;
    spi_inst_t *spi;

public:
    SPI(spi_inst_t *spi_, uint baudrate, uint rx_pin, uint sck_pin, uint tx_pin);
    ~SPI();
    void write_register(uint cs_pin, uint8_t reg, uint8_t data);
    void read_registers(uint cs_pin, uint8_t reg, uint8_t *buf, uint16_t len);

};

#endif /* __SPI_H__ */
