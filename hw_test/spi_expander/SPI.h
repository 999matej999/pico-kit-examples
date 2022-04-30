#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "hardware/spi.h"

class SPI
{

private:
    spi_inst_t *spi;

public:
    SPI(spi_inst_t *spi_, uint baudrate, uint rx_pin, uint sck_pin, uint tx_pin);
    ~SPI();
    void write_register(uint cs_pin, const uint8_t* send_buf, size_t send_len);
    void read_registers(uint cs_pin, const uint8_t* send_buf, size_t send_len, uint8_t *recv_buf, size_t recv_len);

};

#endif /* __SPI_H__ */
